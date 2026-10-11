#pragma once

#include <Engine/Component.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>

#include <glm/glm.hpp>

#include <memory>

struct Sprite : public Component{
private:
public:
    std::shared_ptr<Texture2D> texture;
    glm::vec4 color = glm::vec4(1);
};