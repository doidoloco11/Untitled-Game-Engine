#pragma once

#include <Engine/Component.hpp>
#include <Engine/AssetManager/Model/Model.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/AssetManager/Shader/Shader.hpp>

#include <memory>

namespace uge{
    struct MeshRenderer : public Component{
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture2D> mainTexture;

        std::shared_ptr<Model> model;
    };
}