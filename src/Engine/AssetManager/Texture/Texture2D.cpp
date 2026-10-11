#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/AssetManager/Texture/TextureHandle.hpp>

#include <glad/glad.h>

#include <stdexcept>
#include <string.h>
#include <vector>

Texture2D::Texture2D(GLint internalFormat, const int& width, const int& height) : width(width), height(height){
    data.assign(width * height, glm::vec4(0));

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, 1, internalFormat, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data.data());

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

Texture2D::~Texture2D(){
    glDeleteTextures(1, &texture);
}

void Texture2D::setPixels(const std::vector<glm::vec4> &pixels){
    if (pixels.size() != data.size()) throw std::out_of_range("pixels deve ter o mesmo tamanho de data");

    memcpy(data.data(), pixels.data(), pixels.size() * sizeof(glm::vec4));
}

TextureHandle Texture2D::createSamplerHandle(const std::vector<int>& params){
    GLuint sampler;
    glCreateSamplers(1, &sampler);
    for (int i = 0; i < params.size(); i+=2) {
        glSamplerParameteri(sampler, params[i], params[i + 1]);
    }
    GLuint64 handle = glGetTextureSamplerHandleARB(texture, sampler);
    glMakeTextureHandleResidentARB(handle);
    return TextureHandle{.handle = handle, .sampler = sampler};
}