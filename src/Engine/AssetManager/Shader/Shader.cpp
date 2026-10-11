#include <Engine/AssetManager/Shader/Shader.hpp>

#include <glad/glad.h>

#include <string>

Shader::Shader(){
    Id = glCreateProgram();
}

Shader::~Shader(){
    uniformIds.clear();
    glDeleteProgram(Id);
}

const GLint& Shader::getUniform(const std::string& uName){
    if (uniformIds.find(uName) == uniformIds.end()){
        GLint loc = glGetUniformLocation(Id, uName.c_str());
        uniformIds[uName] = loc;
    }
    return uniformIds[uName];
}