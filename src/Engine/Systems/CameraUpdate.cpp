#include <Engine/Systems/CameraUpdate.hpp>
#include <Engine/Components/Camera.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Engine.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

void CameraUpdate::onStart(std::vector<Entity>& entities){
    for (Entity& entity : entities) {
        if (entity.getComponent<Camera>() && entity.getComponent<Transform>() && !entity.willBeDeleted){
            cameraUUID = entity.uuid;
            return;
        }
    }
}

void CameraUpdate::onUpdate(std::vector<Entity>& entities, const float& deltaTime){
    if (cameraUUID.has_value()){
        for (Entity& entity : entities) {
            if (entity.uuid == cameraUUID){
                Camera* camera = entity.getComponent<Camera>();
                Transform* transform = entity.getComponent<Transform>();

                int width, height;
                glfwGetFramebufferSize(engine->window , &width, &height);

                float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

                glm::mat4 m_proj;

                if (camera->type == Camera::Perspective){
                    m_proj = glm::perspectiveLH(camera->fov, aspectRatio, camera->nearClipPlane, camera->farClipPlane);
                }else{
                    float sin = glm::sin(camera->fov);
                    m_proj = glm::orthoLH(-1.f * aspectRatio * sin, 1.f * aspectRatio * sin, -1.f * sin, 1.f * sin, camera->nearClipPlane, camera->farClipPlane);
                }

                glm::mat4 m_look = glm::lookAtLH(transform->position, transform->position + transform->forward(), transform->up());

                Camera::CameraData data{
                    .m_proj = m_proj,
                    .m_look = m_look,
                    .nearClipPlane = camera->nearClipPlane,
                    .farClipPlane = camera->farClipPlane,
                    .fov = camera->fov,
                    .aspectRatio = aspectRatio
                };

                glNamedBufferSubData(camera->ubo, 0, sizeof(Camera::CameraData), &data);
                glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera->ubo);

                return;
            }
        }
        cameraUUID.reset();
    }

    for (Entity& entity : entities) {
        if (entity.getComponent<Camera>() && entity.getComponent<Transform>() && !entity.willBeDeleted){
            cameraUUID = entity.uuid;
            return;
        }
    }
}