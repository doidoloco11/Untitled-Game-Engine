#include <Engine/AssetManager/Texture/Texture2DLoader.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/AssetManager/Asset.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <stdexcept>
#include <memory>
#include <vector>
#include <filesystem>

std::shared_ptr<Asset> Texture2DLoader::Load(const std::filesystem::path &filePath){
    int width, height, channels;

    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!image) throw std::runtime_error("falhou em abrir: " + filePath.string());

    std::vector<glm::vec4> colors;
    colors.assign(width * height, glm::vec4(0));

    for (int i = 0; i < width * height; i++) {
        colors[i].a = 1;
        colors[i].r = static_cast<float>(image[i * channels + 0]) / 255;
        if (channels > 1) colors[i].g = static_cast<float>(image[i * channels + 1]) / 255;
        if (channels > 2) colors[i].b = static_cast<float>(image[i * channels + 2]) / 255;
        if (channels > 3) colors[i].a = static_cast<float>(image[i * channels + 3]) / 255;
    }

    stbi_image_free(image);

    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(GL_RGBA8, width, height);
    texture->setPixels(colors);
    texture->Apply();
    return texture;
}