#pragma once

#include <Engine/Component.hpp>
#include <Engine/AssetManager/Model/Model.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <cstdint>

namespace uge{
    struct Collider : public Component{
    private:
    public:
        glm::vec3 sizeMultiplier = glm::vec3(1);
        std::shared_ptr<Model> model;

        struct CollisionData{
            uint64_t uuid;

            glm::vec3 normal;
            glm::vec3 midPoint;
            float depth;
        };

        std::vector<CollisionData> collisions;
    };
}