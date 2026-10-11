#include <Engine/Systems/SpriteRenderer.hpp>
#include <Engine/AssetManager/Shader/Shader.hpp>
#include <Engine/AssetManager/Texture/Texture2D.hpp>
#include <Engine/AssetManager/AssetManager.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Components/Sprite.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>

void SpriteRenderer::onAwake(){
    spriteShader = std::static_pointer_cast<Shader>(engine->assetManager->LoadAsset("shaders/sprite.shader"));
    whiteTexture = std::make_unique<Texture2D>(GL_RGBA8, 1, 1);
    whiteTexture->setPixel(0, 0, glm::vec4(1));
    whiteTexture->Apply();
    glCreateVertexArrays(1, &vao);
}

void SpriteRenderer::onTerminate(std::vector<Entity>&) {
    glDeleteVertexArrays(1, &vao);
}

void SpriteRenderer::onRender(std::vector<Entity>& entities) {
    glUseProgram(spriteShader->getId());

    glBindVertexArray(vao);

    for (Entity& entity : entities) {
        Sprite* sprite = entity.getComponent<Sprite>();
        Transform* transform = entity.getComponent<Transform>();

        if (sprite && transform){
            GLuint texture;

            if (sprite->texture){
                texture = sprite->texture->getTexture();
            }else{
                texture = whiteTexture->getTexture();;
            }

            const glm::vec3& position = transform->position;
            const glm::vec2& scale = transform->scale;

            const glm::vec4& color = sprite->color;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glUniform1i(spriteShader->getUniform("mainTexture"), 0);
            glUniform3f(spriteShader->getUniform("position"), position.x, position.y, position.z);
            glUniform2f(spriteShader->getUniform("scale"), scale.x, scale.y);
            glUniform1f(spriteShader->getUniform("angle"), transform->toEulerAngles().z);
            glUniform4f(spriteShader->getUniform("color"), color.x, color.y, color.z, color.w);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
}