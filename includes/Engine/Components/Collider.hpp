#pragma once

#include "glm/ext/vector_float3.hpp"
#include <Engine/Component.hpp>

#include <cstdint>
#include <glm/glm.hpp>

#include <array>

struct Collider : public Component{
private:
public:
    glm::vec3 sizeMultiplier = glm::vec3(1);

    struct Triangle{
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;

        const std::array<glm::vec3, 4> getAxis(const glm::mat4& m_model);
        void getProjection(const glm::mat4& m_model, const glm::vec3& axis, float& projA, float& projB);

        const glm::vec3 getCentroid(const glm::mat4& m_model) {return m_model * glm::vec4((v0 + v1 + v2) / 3.f, 1);}
    };
    std::vector<Triangle> triangles = { Triangle {glm::vec3(-1, -1, 0), glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0)},
                                        Triangle {glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0)}};

    struct CollisionData{
        uint64_t uuid;

        glm::vec3 normal;
        glm::vec3 midPoint;
        float depth;
    };

    std::vector<CollisionData> collisions;
};