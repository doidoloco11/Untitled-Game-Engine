#pragma once

#include <Engine/System.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Components/Collider.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <queue>
#include <utility>

namespace uge{
    class SATCollision : public System{
    private:
        std::vector<glm::mat4> m_models;
        std::vector<Collider*> colliders;
        std::vector<uint64_t> uuid;

        std::queue<std::pair<unsigned int, unsigned int>> aabbTest;

        struct TriangleTestData{
            unsigned int first_index;
            unsigned int index_count;
        };
        std::vector<std::pair<TriangleTestData, TriangleTestData>> trianglesTest;
    public:
        using System::System;

        void onUpdate(std::vector<Entity>&, const float& deltaTime) override;
    };
}