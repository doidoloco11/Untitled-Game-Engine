#pragma once

#include <Engine/System.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Components/Collider.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Components/PhysicsBody.hpp>

#include <vector>
#include <unordered_map>
#include <cstdint>

class Physics : public System{
private:
    std::unordered_map<uint64_t, int> uuidToLocalIndex;

    std::vector<Collider*> colliders;
    std::vector<Transform*> transforms;
    std::vector<PhysicsBody*> physicsBodys;
public:
    using System::System;

    void onUpdate(std::vector<Entity>&, const float& deltaTime) override;
};