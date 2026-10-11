#pragma once

#include <Engine/Component.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>

struct Transform : public Component{
private:
    Transform* parent = nullptr;
    std::vector<Transform*> children;
public:

    glm::vec3 position = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
    glm::quat rotation = glm::identity<glm::quat>();

    void setParent(Transform* parent);
    void Delete() override;

    Transform* getParent() {return parent;}
    std::vector<Transform*> getChildren() {return children;}

    const glm::mat4 getRotationMatrix(glm::mat4 matrix=glm::identity<glm::mat4>(), Transform* parent=nullptr) const {
        if (parent) parent->getRotationMatrix(matrix, parent->parent);
        matrix = matrix * glm::mat4_cast(rotation);
        return matrix;
    }

    const glm::mat4 Matrix() const {
        glm::mat4 matrix = glm::identity<glm::mat4>();
        if (parent) matrix = parent->Matrix() * matrix;
        matrix = glm::translate(matrix, position);
        matrix = getRotationMatrix(matrix);
        matrix = glm::scale(matrix, scale);
        return matrix;
    }

    const glm::vec3 forward() const {return getRotationMatrix(glm::identity<glm::mat4>(), parent) * glm::vec4(0, 0, 1, 1);}
    const glm::vec3 up() const {return getRotationMatrix(glm::identity<glm::mat4>(), parent) * glm::vec4(0, 1, 0, 1);}
    const glm::vec3 right() const {return getRotationMatrix(glm::identity<glm::mat4>(), parent) * glm::vec4(1, 0, 0, 1);}

    const glm::vec3 toEulerAngles() {return glm::eulerAngles(rotation);}
    void fromEulerAngles(const glm::vec3& angle) {rotation = glm::quat(angle);}
};