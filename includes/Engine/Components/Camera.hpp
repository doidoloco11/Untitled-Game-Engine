#pragma once

#include <Engine/Component.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct Camera : public Component{
private:
public:
    struct CameraData{
    alignas(16)    glm::mat4 m_proj;
    alignas(16)    glm::mat4 m_look;
    alignas(4)    float nearClipPlane;
    alignas(4)    float farClipPlane;
    alignas(4)    float fov;
    alignas(4)    float aspectRatio;
    };

    enum CameraType{
        Perspective,
        Orthographic
    };

    GLuint ubo;

    CameraType type;

    float nearClipPlane;
    float farClipPlane;
    float fov;

    Camera(){
        type = Perspective;
        nearClipPlane = 1e-2f;
        farClipPlane = 1e+4f;
        fov = glm::pi<float>() / 3.f;
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, sizeof(CameraData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    }

    void Delete() override{
        Component::Delete();
        glDeleteBuffers(1, &ubo);
    }
};