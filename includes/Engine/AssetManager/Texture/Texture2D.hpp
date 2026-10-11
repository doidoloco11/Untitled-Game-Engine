#pragma once

#include <Engine/AssetManager/Asset.hpp>
#include <Engine/AssetManager/Texture/TextureHandle.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Texture2D : public Asset{
private:
    int width, height;

    std::vector<glm::vec4> data;

    GLuint texture;
public:
    Texture2D(GLint internalFormat, const int& width, const int& height);
    ~Texture2D();

    void setPixel(int x, int y, glm::vec4 color) {data[x + width * y] = color;};
    void setPixels(const std::vector<glm::vec4>& pixels);
    void Apply() {glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data.data());}
    void Sync() {glGetTextureSubImage(texture, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_FLOAT, data.size(), data.data());}

    void getSize(int& width, int& height) {width = this->width; height = this->height;}
    TextureHandle createSamplerHandle(const std::vector<int>& params);
    const glm::vec4& getPixel(int x, int y) {return data[x + width * y];};
    const std::vector<glm::vec4>& getPixels() {return data;}

    const GLuint& getTexture() {return texture;}
};