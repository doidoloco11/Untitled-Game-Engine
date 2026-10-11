#pragma once

#include <Engine/Component.hpp>

#include <glm/glm.hpp>

struct PhysicsBody : public Component{
public:
    enum PhysicsBodyType{
        Rigid,
        Kinematic,
        Static
    };

    glm::vec3 linearVelocity = glm::vec3(0);
    glm::vec3 angularVelocity = glm::vec3(0);

    float dragCoefficient = 0;
    float gravity = 10;

    float mass = 1;
    float bounciness = 1;
    PhysicsBodyType type = Rigid;
};