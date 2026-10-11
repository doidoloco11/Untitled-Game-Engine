#pragma once

#include <Engine/AssetManager/Asset.hpp>

#include <glad/glad.h>

#include <string>
#include <unordered_map>

struct Shader : public Asset {
private:
    std::unordered_map<std::string, GLint> uniformIds;
    GLuint Id;
public: 
    Shader();
    ~Shader();

    inline const GLuint& getId() {return Id;};
    const GLint& getUniform(const std::string& uName);
};