#include <Engine/AssetManager/Shader/ShaderLoader.hpp>
#include <Engine/AssetManager/Shader/Shader.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <glad/glad.h>

#include <memory>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

GLuint LoadShader(const std::filesystem::path& shaderPath, GLenum shaderType){
    GLuint shaderId = glCreateShader(shaderType);

    std::ifstream file(shaderPath, std::ios::ate);
    if (!file.is_open()){
        throw std::runtime_error("falhou em abrir arquivo: " + shaderPath.string());
    }

    size_t size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    
    std::string source(size, '\0');
    file.read(&source[0], size);

    file.close();
    const char* src = source.c_str();
    
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        glDeleteShader(shaderId);
        throw std::runtime_error(infoLog);
    }
    return shaderId;
}

struct shaderInfo{
    std::filesystem::path filePath;
    GLenum shaderType;
};

std::shared_ptr<Asset> ShaderLoader::Load(const std::filesystem::path& filePath){
    std::ifstream file(filePath);

    if (!file.is_open()) throw std::runtime_error("falhou em abrir arquivo: " + filePath.string());

    std::vector<shaderInfo> infos;

    std::string line;
    while (std::getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::filesystem::path path = line.substr(5);
        if (line.rfind("vert", 0) == 0) infos.push_back(shaderInfo {.filePath = path, .shaderType = GL_VERTEX_SHADER});
        else if (line.rfind("frag", 0) == 0) infos.push_back(shaderInfo {.filePath = path, .shaderType = GL_FRAGMENT_SHADER});
        else if (line.rfind("comp", 0) == 0) infos.push_back(shaderInfo {.filePath = path, .shaderType = GL_COMPUTE_SHADER});
        else if (line.rfind("teco", 0) == 0) infos.push_back(shaderInfo {.filePath = path, .shaderType = GL_TESS_CONTROL_SHADER});
        else if (line.rfind("teev", 0) == 0) infos.push_back(shaderInfo {.filePath = path, .shaderType = GL_TESS_EVALUATION_SHADER});
        else std::cerr << "tipo: " << line.substr(0, 4) << " não encontrado";
    }
    file.close();

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(); 

    std::vector<GLuint> shaders;
    for (size_t i = 0; i < infos.size(); i++){
        GLuint s = LoadShader(infos[i].filePath, infos[i].shaderType);
        
        glAttachShader(shader->getId(), s);

        shaders.push_back(s);
    }
    infos.clear();

    glLinkProgram(shader->getId());

    for (size_t i = 0; i < shaders.size(); i++){
        glDetachShader(shader->getId(), shaders[i]);
        glDeleteShader(shaders[i]);
    }

    int success;
    glGetProgramiv(shader->getId(), GL_LINK_STATUS, &success);
    if (!success){
        char infoLog[512];
        glGetProgramInfoLog(shader->getId(), 512, nullptr, infoLog);
        glDeleteProgram(shader->getId());
        throw std::runtime_error(infoLog);
    }

    return shader;
}