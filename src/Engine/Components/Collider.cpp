#include <Engine/Components/Collider.hpp>

#include <glm/glm.hpp>

#include <array>

const std::array<glm::vec3, 4> Collider::Triangle::getAxis(const glm::mat4& m_model){
    glm::vec3 gv0 = m_model * glm::vec4(v0, 1);
    glm::vec3 gv1 = m_model * glm::vec4(v1, 1);
    glm::vec3 gv2 = m_model * glm::vec4(v2, 1);

    glm::vec3 e0 = glm::normalize(gv1 - gv0);
    glm::vec3 e1 = glm::normalize(gv2 - gv0);
    glm::vec3 e2 = glm::normalize(gv2 - gv1);

    glm::vec3 normal = glm::normalize(glm::cross(e0, e1));
    return std::array<glm::vec3, 4> {e0, e1, e2, normal};
}

void Collider::Triangle::getProjection(const glm::mat4& m_model, const glm::vec3& axis, float& projA, float& projB){
    glm::vec3 gv0 = m_model * glm::vec4(v0, 1);
    glm::vec3 gv1 = m_model * glm::vec4(v1, 1);
    glm::vec3 gv2 = m_model * glm::vec4(v2, 1);

    float p0 = glm::dot(gv0, axis);
    float p1 = glm::dot(gv1, axis);
    float p2 = glm::dot(gv2, axis);

    projA = std::min(p0, std::min(p1, p2));
    projB = std::max(p0, std::max(p1, p2));
}