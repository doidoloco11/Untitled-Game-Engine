#include "glm/geometric.hpp"
#include <Engine/Systems/Physics.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Components/Collider.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Components/PhysicsBody.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

void Physics::onUpdate(std::vector<Entity>& entities, const float &deltaTime){
    uuidToLocalIndex.clear();

    colliders.clear();
    transforms.clear();
    physicsBodys.clear();

    for (Entity& entity : entities) {
        Collider* c = entity.getComponent<Collider>();
        Transform* t = entity.getComponent<Transform>();
        PhysicsBody* pb = entity.getComponent<PhysicsBody>();
        if (c && t && pb){
            uuidToLocalIndex[entity.uuid] = colliders.size();
            colliders.push_back(c);
            transforms.push_back(t);
            physicsBodys.push_back(pb);
        }
    }

    for (int i = 0; i < colliders.size(); i++) {
        for (Collider::CollisionData& collision : colliders[i]->collisions) {
            auto it = uuidToLocalIndex.find(collision.uuid);
            if (it != uuidToLocalIndex.end() && it->second > i){
                int id = it->second;

                Transform* thisTransform = transforms[i];
                PhysicsBody* thisBody = physicsBodys[i];

                Transform* otherTransform = transforms[id];
                PhysicsBody* otherBody = physicsBodys[id];

                glm::vec3 relVel = thisBody->linearVelocity - otherBody->linearVelocity;

                float velAlongNormal = glm::dot(relVel, collision.normal);

                if (velAlongNormal > 0) continue;

                float meanBounciness = std::sqrt(thisBody->bounciness * otherBody->bounciness);

                
                float invMassA = (thisBody->type == PhysicsBody::Static) ? 0.0f : (1.0f / thisBody->mass);
                float invMassB = (otherBody->type == PhysicsBody::Static) ? 0.0f : (1.0f / otherBody->mass);
                float totalInvMass = invMassA + invMassB;

                if (totalInvMass == 0.0f) continue; 

                float impulseScalar = -(1.0f + meanBounciness) * velAlongNormal;
                impulseScalar /= totalInvMass;

                glm::vec3 impulse = impulseScalar * collision.normal;

                if (thisBody->type != PhysicsBody::Static){
                    thisBody->linearVelocity += invMassA * impulse;
                    
                    float distrib = (otherBody->type == PhysicsBody::Static) ? 1.0f : (invMassA / totalInvMass);
                    thisTransform->position += collision.normal * collision.depth * distrib;
                }

                if (otherBody->type != PhysicsBody::Static){
                    otherBody->linearVelocity -= invMassB * impulse;
                    
                    float distrib = (thisBody->type == PhysicsBody::Static) ? 1.0f : (invMassB / totalInvMass);
                    otherTransform->position -= collision.normal * collision.depth * distrib;
                }
            }
        }
    }

    for (int i = 0; i < physicsBodys.size(); i++) {
        Transform* transform = transforms[i];
        PhysicsBody* physicsBody = physicsBodys[i];
        if (physicsBody->type != PhysicsBody::Static){
            if (physicsBody->type == PhysicsBody::Rigid){
                physicsBody->linearVelocity += glm::vec3(0, -physicsBody->gravity, 0) * deltaTime;
            }
            float drag = std::pow(std::min(1 - physicsBody->dragCoefficient, 0.99999f), deltaTime);;
            physicsBody->linearVelocity *= drag;
            physicsBody->angularVelocity *= drag;

            glm::quat qDot = 0.5f * glm::quat(0.0f, physicsBody->angularVelocity.x, physicsBody->angularVelocity.y, physicsBody->angularVelocity.z) * transform->rotation;

            transform->position += physicsBody->linearVelocity * deltaTime;
            transform->rotation += qDot * deltaTime;
            transform->rotation = glm::normalize(transform->rotation);
        }
    }  
}