#pragma once

#include <Engine/System.hpp>
#include <Engine/AssetManager/Shader/Shader.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/Entity.hpp>

#include <memory>
#include <vector>

class SpriteRenderer : public System{
private:
    std::shared_ptr<Shader> spriteShader;
    std::unique_ptr<Texture2D> whiteTexture;
    GLuint vao;

public:
    using System::System;

    void onAwake() override;
    void onRender(std::vector<Entity>&) override;
    void onTerminate(std::vector<Entity>&) override;
};