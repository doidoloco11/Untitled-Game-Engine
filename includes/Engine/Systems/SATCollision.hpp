#pragma once

#include <Engine/System.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Components/Collider.hpp>

#include <glm/glm.hpp>

#include <vector>

class SATCollision : public System{
private:
    std::vector<glm::mat4> m_models;
    std::vector<Collider*> colliders;
    std::vector<uint64_t> uuid;
public:
    using System::System;

    void onUpdate(std::vector<Entity>&, const float& deltaTime) override;
};