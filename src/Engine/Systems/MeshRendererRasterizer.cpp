#include <Engine/Systems/MeshRendererRasterizer.hpp>
#include <Engine/Components/MeshRenderer.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/AssetManager/Shader/Shader.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Entity.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

namespace uge{
    void MeshRendererRasterizer::onAwake(){
        whiteTexture = std::make_unique<Texture2D>(GL_RGBA8, 1, 1);
        whiteTexture->setPixel(0, 0, glm::vec4(1));
        whiteTexture->Apply();
    }

    void MeshRendererRasterizer::onRender(std::vector<Entity>& entities){
        for (Entity& entity : entities) {
            Transform* transform = entity.getComponent<Transform>();
            MeshRenderer* renderer = entity.getComponent<MeshRenderer>();
            if (transform && renderer){
                glUseProgram(renderer->shader->getId());

                glBindVertexArray(renderer->model->getVAO());

                glUniformMatrix4fv(renderer->shader->getUniform("m_model"), 1, GL_FALSE, glm::value_ptr(transform->Matrix()));

                glDrawElements(GL_TRIANGLES, renderer->model->indices.size(), GL_UNSIGNED_INT, nullptr);
                
            }
        }
    }
}