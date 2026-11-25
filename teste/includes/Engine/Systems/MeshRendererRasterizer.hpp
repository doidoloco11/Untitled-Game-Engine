#pragma once

#include <Engine/System.hpp>
#include <Engine/Entity.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>

#include <memory>
#include <vector>

namespace uge{
    class MeshRendererRasterizer : public System{
    private:
        std::unique_ptr<Texture2D> whiteTexture;

        void onAwake() override;
    public:
        using System::System;
        void onRender(std::vector<Entity>&) override;
    };
}