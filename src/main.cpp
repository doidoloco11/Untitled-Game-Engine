#include <Engine/Engine.hpp>
#include <Engine/AssetManager/Shader/ShaderLoader.hpp>
#include <Engine/AssetManager/Texture/Texture2DLoader.hpp>
#include <Engine/Systems/CameraUpdate.hpp>
#include <Engine/Systems/SpriteRenderer.hpp>
#include <Engine/Systems/SATCollision.hpp>
#include <Engine/Systems/Physics.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Components/Camera.hpp>
#include <Engine/Components/Sprite.hpp>
#include <Engine/Components/Collider.hpp>
#include <Engine/Components/PhysicsBody.hpp>
#include <memory>

int main(){
    std::unique_ptr<Engine> engine = Engine::CreateEngineInstance(800, 600, "janela legal");

    engine->assetManager->AddLoader<ShaderLoader>(".shader");
    engine->assetManager->AddLoader<Texture2DLoader>(".jpg");

    engine->AddSystem<CameraUpdate>();
    engine->AddSystem<SpriteRenderer>();
    engine->AddSystem<SATCollision>();
    engine->AddSystem<Physics>();

    Entity& camera = engine->CreateEntity();
    camera.addComponent<Transform>();
    camera.addComponent<Camera>();

    camera.getComponent<Transform>()->position = glm::vec3(0, 0, -5);

    Entity& sprite = engine->CreateEntity();
    sprite.addComponent<Transform>();
    sprite.addComponent<Sprite>();
    sprite.addComponent<Collider>();
    sprite.addComponent<PhysicsBody>();

    Transform* t = sprite.getComponent<Transform>();

    t->position = glm::vec3(0, -2.f, 0);
    t->scale = glm::vec3(3.f, 0.7f, 1);

    sprite.getComponent<Sprite>()->texture = std::static_pointer_cast<Texture2D>(engine->assetManager->LoadAsset("imagens/galho.jpg"));

    sprite.getComponent<PhysicsBody>()->type = PhysicsBody::Static;

    Entity& sprite2 = engine->CreateEntity();
    sprite2.addComponent<Transform>();
    sprite2.addComponent<Sprite>();
    sprite2.addComponent<Collider>();
    sprite2.addComponent<PhysicsBody>();

    t = sprite2.getComponent<Transform>();

    t->position = glm::vec3(0, 2, 0);
    t->scale = glm::vec3(1.f, 1.f, 1);

    sprite2.getComponent<Sprite>()->texture = std::static_pointer_cast<Texture2D>(engine->assetManager->LoadAsset("imagens/macaco.jpg"));

    engine->run();

    return 0;
}