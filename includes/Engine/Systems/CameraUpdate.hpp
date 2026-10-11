#pragma once

#include <Engine/System.hpp>
#include <Engine/Entity.hpp>

#include <vector>
#include <cstdint>
#include <optional>

class CameraUpdate : public System{
public:
    std::optional<uint64_t> cameraUUID;

    using System::System;

    void onStart(std::vector<Entity>&) override;
    void onUpdate(std::vector<Entity>&, const float& deltaTime) override;
};