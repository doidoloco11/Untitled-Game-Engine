#include <Engine/Systems/SATCollision.hpp>
#include <Engine/Components/Collider.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/AssetManager/Model/BVH.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Entity.hpp>

#include <array>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>
#include <utility>

namespace uge{
    using namespace glm;
    std::array<vec3, 4> getTriangleAxes(vec3 v0, vec3 v1, vec3 v2){
        vec3 e0 = normalize(v1 - v0);
        vec3 e1 = normalize(v2 - v0);
        vec3 e2 = normalize(v2 - v1);

        vec3 normal = normalize(cross(e1, e0));

        mat3 rot = rotate(identity<mat4>(), pi<float>() / 2.f, normal);

        return std::array<vec3, 4> {normal, e0 * rot, e1 * rot, e2 * rot};
    }

    void getTriangleProjection(vec3 v0, vec3 v1, vec3 v2, vec3 sizeMultiplier, vec3 axis, float& minProj, float& maxProj){
        float proj0 = dot(v0 * sizeMultiplier, axis);
        float proj1 = dot(v1 * sizeMultiplier, axis);
        float proj2 = dot(v2 * sizeMultiplier, axis);

        minProj = min(proj0, min(proj1, proj2));
        maxProj = max(proj0, max(proj1, proj2));
    }

    bool checkAABBOverlapLocal(const vec3& minA, const vec3& maxA, 
                               const vec3& minB, const vec3& maxB, 
                               const mat4& mB_to_A) {
        
        vec3 extB = (maxB - minB) * 0.5f;
        vec3 centerB_localB = (minB + maxB) * 0.5f;
        
        vec3 centerB_localA = vec3(mB_to_A * vec4(centerB_localB, 1.0f));
        vec3 centerA = (minA + maxA) * 0.5f;
        vec3 extA = (maxA - minA) * 0.5f;

        vec3 T = centerB_localA - centerA;

        mat3 R = mat3(mB_to_A);
        mat3 absR;
        
        for(int i=0; i<3; ++i)
            for(int j=0; j<3; ++j)
                absR[i][j] = glm::abs(R[i][j]) + FLT_EPSILON;

        float ra, rb;

        for (int i = 0; i < 3; i++) {
            ra = extA[i];
            rb = extB[0] * absR[0][i] + extB[1] * absR[1][i] + extB[2] * absR[2][i];
            if (glm::abs(T[i]) > ra + rb) return false;
        }

        for (int i = 0; i < 3; i++) {
            ra = extA[0] * absR[i][0] + extA[1] * absR[i][1] + extA[2] * absR[i][2];
            rb = extB[i];
            if (glm::abs(dot(T, vec3(R[i][0], R[i][1], R[i][2]))) > ra + rb) return false;
        }

        return true;
    }

    void SATCollision::onUpdate(std::vector<Entity>& entities, const float &deltaTime){
        m_models.clear();
        colliders.clear();
        uuid.clear();

        for (Entity& entity : entities) {
            Transform* t = entity.getComponent<Transform>();
            Collider* c = entity.getComponent<Collider>();

            if (t && c){
                c->collisions.clear();
                m_models.push_back(t->Matrix());
                colliders.push_back(c);
                uuid.push_back(entity.uuid);
            }
        }

        for (int i = 0; i < uuid.size() - 1; i++) {
            for (int j = i + 1; j < uuid.size(); j++) {
                Collider* thisCollider = colliders[i];
                Collider* otherCollider = colliders[j];
                bool foundCollision = false;

                mat4 invModelA = glm::inverse(m_models[i]);
                
                mat4 mB_to_A = invModelA * m_models[j]; 

                mB_to_A = glm::scale(mB_to_A, otherCollider->sizeMultiplier);

                trianglesTest.clear();
                aabbTest.push(std::make_pair(0, 0));
                while (!aabbTest.empty()){
                    unsigned int thisIndex = aabbTest.front().first;
                    unsigned int otherIndex = aabbTest.front().second;
                    
                    BVH::Node& thisNode = thisCollider->model->bvh.nodes[thisIndex];
                    BVH::Node& otherNode = otherCollider->model->bvh.nodes[otherIndex];
                    aabbTest.pop();

                    if (checkAABBOverlapLocal(thisNode.min, thisNode.max, 
                                         otherNode.min, otherNode.max, 
                                         mB_to_A)){
                        if (!thisNode.isLeafNode() && !otherNode.isLeafNode()){
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 0, otherNode.childrenIndex + 0));
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 0, otherNode.childrenIndex + 1));
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 1, otherNode.childrenIndex + 0));
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 1, otherNode.childrenIndex + 1));
                        }else if (!thisNode.isLeafNode() && otherNode.isLeafNode()){
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 0, otherIndex));
                            aabbTest.push(std::make_pair(thisNode.childrenIndex + 1, otherIndex));
                        }else if (thisNode.isLeafNode() && !otherNode.isLeafNode()){
                            aabbTest.push(std::make_pair(thisIndex, otherNode.childrenIndex + 0));
                            aabbTest.push(std::make_pair(thisIndex, otherNode.childrenIndex + 1));
                        }else{
                            trianglesTest.push_back(std::make_pair(TriangleTestData {.first_index = thisNode.childrenIndex, .index_count = thisNode.trianglesindexCount},
                                                                      TriangleTestData {.first_index = otherNode.childrenIndex, .index_count = otherNode.trianglesindexCount}));
                        }
                    }
                }
                for (const auto& testData : trianglesTest) {
                    for (int k = testData.first.first_index; k < testData.first.first_index + testData.first.index_count - 1; k+=3) {
                        vec3 thisv0, thisv1, thisv2;
                        thisv0 = m_models[i] * vec4(thisCollider->model->vertices[thisCollider->model->indices[k + 0]], 1);
                        thisv1 = m_models[i] * vec4(thisCollider->model->vertices[thisCollider->model->indices[k + 1]], 1);
                        thisv2 = m_models[i] * vec4(thisCollider->model->vertices[thisCollider->model->indices[k + 2]], 1);
                        for (int l = testData.second.first_index; l < testData.second.first_index + testData.second.index_count - 1; l+=3) {
                            std::array<vec3, 4> axes;
                            
                            float minDepth = FLT_MAX;
                            float midDepth;
                            vec3 normal;
                            vec3 triangleCentroid;

                            vec3 otherv0, otherv1, otherv2;
                            otherv0 = m_models[j] * vec4(otherCollider->model->vertices[otherCollider->model->indices[l + 0]], 1);
                            otherv1 = m_models[j] * vec4(otherCollider->model->vertices[otherCollider->model->indices[l + 1]], 1);
                            otherv2 = m_models[j] * vec4(otherCollider->model->vertices[otherCollider->model->indices[l + 2]], 1);

                            axes = getTriangleAxes(thisv0, thisv1, thisv2);
                            bool overlapAll = true;
                            for (const vec3& axis : axes) {
                                float proj1A, proj1B, proj2A, proj2B;
                                getTriangleProjection(thisv0, thisv1, thisv2, thisCollider->sizeMultiplier, axis, proj1A, proj1B);

                                getTriangleProjection(otherv0, otherv1, otherv2, otherCollider->sizeMultiplier, axis, proj2A, proj2B);

                                if (proj1A > proj2B || proj2A > proj1B){
                                    overlapAll = false;
                                    break;
                                }else{
                                    float A1B2 = proj2B - proj1A;
                                    float A2B1 = proj1B - proj2A;

                                    float depth = std::min(A1B2, A2B1);

                                    if (depth < minDepth && depth > 0){
                                        minDepth = depth;
                                        normal = axis * sign((proj2A + proj2B) - (proj1A + proj1B));
                                        midDepth = (A1B2 + A2B1) / 2.f;
                                        triangleCentroid = (thisv0 + thisv1 + thisv2) / 3.f;
                                    }
                                }
                            }
                            if (!overlapAll) continue;

                            axes = getTriangleAxes(otherv0, otherv1, otherv2);
                            for (const vec3& axis : axes) {
                                float proj1A, proj1B, proj2A, proj2B;

                                getTriangleProjection(thisv0, thisv1, thisv2, thisCollider->sizeMultiplier, axis, proj1A, proj1B);

                                getTriangleProjection(otherv0, otherv1, otherv2, otherCollider->sizeMultiplier, axis, proj2A, proj2B);

                                if (proj1A > proj2B || proj2A > proj1B){
                                    overlapAll = false;
                                    break;
                                }else{
                                    float A1B2 = proj2B - proj1A;
                                    float A2B1 = proj1B - proj2A;

                                    float depth = std::min(A1B2, A2B1);

                                    if (depth < minDepth && depth > 0){
                                        minDepth = depth;
                                        normal = axis * sign((proj2A + proj2B) - (proj1A + proj1B));
                                        midDepth = (A1B2 + A2B1) / 2.f;
                                        triangleCentroid = (otherv0 + otherv1 + otherv2) / 3.f;
                                    }
                                }
                            }

                            if (overlapAll){
                                float dist = midDepth - dot(triangleCentroid, normal);
                                vec3 midpoint = triangleCentroid + normal * dist;

                                thisCollider->collisions.push_back(Collider::CollisionData {
                                    .uuid = uuid[j],
                                    .normal = -normal,
                                    .midPoint = midpoint,
                                    .depth = minDepth
                                });

                                otherCollider->collisions.push_back(Collider::CollisionData {
                                    .uuid = uuid[i],
                                    .normal = normal,
                                    .midPoint = midpoint,
                                    .depth = minDepth
                                });
                                foundCollision = true;
                                break;
                            }
                        }
                        if (foundCollision) break;
                    }
                    if (foundCollision) break;
                }
            }
        }
    }
}