#include "Engine/AssetManager/Model/Model.hpp"
#include "Engine/Components/Collider.hpp"
#include "Engine/Components/PhysicsBody.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Systems/Physics.hpp"
#include "Engine/Systems/SATCollision.hpp"
#include <Engine/Engine.hpp>
#include <Engine/AssetManager/Shader/ShaderLoader.hpp>
#include <Engine/AssetManager/Texture/Texture2DLoader.hpp>
#include <Engine/AssetManager/Model/ModelLoader.hpp>
#include <Engine/Systems/CameraUpdate.hpp>
#include <Engine/Systems/MeshRendererRasterizer.hpp>
#include <Engine/Components/Transform.hpp>
#include <Engine/Components/Camera.hpp>
#include <Engine/Components/MeshRenderer.hpp>
#include <memory>

using namespace uge;

int main(){
    Engine engine(800, 600, "janela legal");

    engine.assetManager->AddLoader<ShaderLoader>(".shader");
    engine.assetManager->AddLoader<Texture2DLoader>(".jpg");
    engine.assetManager->AddLoader<ModelLoader>(".obj");

    engine.AddSystem<CameraUpdate>();
    engine.AddSystem<SATCollision>();
    engine.AddSystem<Physics>();
    engine.AddSystem<MeshRendererRasterizer>();

    Entity& camera = engine.CreateEntity();
    camera.addComponent<Camera>();
    camera.addComponent<Transform>()->position = glm::vec3(0, 0.3, -2);

    Entity& modelo3D = engine.CreateEntity();
    modelo3D.addComponent<Transform>()->fromEulerAngles(glm::vec3(0, 0, 0));
    MeshRenderer* renderer = modelo3D.addComponent<MeshRenderer>();
    renderer->shader = std::static_pointer_cast<Shader>(engine.assetManager->LoadAsset("shaders/shaderbasico.shader"));
    renderer->model = std::static_pointer_cast<Model>(engine.assetManager->LoadAsset("modelo 3d/fox.obj"));

    modelo3D.addComponent<Collider>()->model = std::static_pointer_cast<Model>(engine.assetManager->LoadAsset("modelo 3d/fox.obj"));
    modelo3D.addComponent<PhysicsBody>();

    Entity& cubo = engine.CreateEntity();
    cubo.addComponent<Transform>()->position.y = -2;
    MeshRenderer* renderercubo = cubo.addComponent<MeshRenderer>();
    renderercubo->shader = std::static_pointer_cast<Shader>(engine.assetManager->LoadAsset("shaders/shaderbasico.shader"));
    renderercubo->model = std::static_pointer_cast<Model>(engine.assetManager->LoadAsset("modelo 3d/cube.obj"));

    cubo.addComponent<Collider>()->model = std::static_pointer_cast<Model>(engine.assetManager->LoadAsset("modelo 3d/cube.obj"));
    cubo.addComponent<PhysicsBody>()->type = PhysicsBody::Static;

    engine.run();

    return 0;
}