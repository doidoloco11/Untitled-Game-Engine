#include <Engine/Systems/SATCollision.hpp>
#include <Engine/Components/Collider.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Entity.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>

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
            for (Collider::Triangle& thisTriangle : thisCollider->triangles) {
                for (Collider::Triangle& otherTriangle : otherCollider->triangles) {
                    std::array<glm::vec3, 4> axes;
                    
                    float minDepth = FLT_MAX;
                    float midDepth;
                    glm::vec3 normal;
                    glm::vec3 triangleCentroid;

                    axes = thisTriangle.getAxis(m_models[i]);
                    bool overlapAll = true;
                    for (const glm::vec3& axis : axes) {
                        float proj1A, proj1B, proj2A, proj2B;
                        thisTriangle.getProjection(m_models[i], axis, proj1A, proj1B);

                        otherTriangle.getProjection(m_models[j], axis, proj2A, proj2B);

                        if (proj1A > proj2B || proj2A > proj1B){
                            overlapAll = false;
                            break;
                        }else{
                            float A1B2 = proj2B - proj1A;
                            float A2B1 = proj1B - proj2A;

                            float depth = std::min(A1B2, A2B1);

                            if (depth < minDepth && depth > 0){
                                minDepth = depth;
                                normal = axis * glm::sign((proj2A + proj2B) - (proj1A + proj1B));
                                midDepth = (A1B2 + A2B1) / 2.f;
                                triangleCentroid = thisTriangle.getCentroid(m_models[i]);
                            }
                        }
                    }
                    if (!overlapAll) continue;

                    axes = otherTriangle.getAxis(m_models[j]);
                    for (const glm::vec3& axis : axes) {
                        float proj1A, proj1B, proj2A, proj2B;

                        thisTriangle.getProjection(m_models[i], axis, proj1A, proj1B);

                        otherTriangle.getProjection(m_models[j], axis, proj2A, proj2B);

                        if (proj1A > proj2B || proj2A > proj1B){
                            overlapAll = false;
                            break;
                        }else{
                            float A1B2 = proj2B - proj1A;
                            float A2B1 = proj1B - proj2A;

                            float depth = std::min(A1B2, A2B1);

                            if (depth < minDepth && depth > 0){
                                minDepth = depth;
                                normal = axis * glm::sign((proj2A + proj2B) - (proj1A + proj1B));
                                midDepth = (A1B2 + A2B1) / 2.f;
                                triangleCentroid = thisTriangle.getCentroid(m_models[j]);
                            }
                        }
                    }

                    if (overlapAll){
                        float dist = midDepth - glm::dot(triangleCentroid, normal);
                        glm::vec3 midpoint = triangleCentroid + normal * dist;

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
        }
    }
}