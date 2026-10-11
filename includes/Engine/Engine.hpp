#pragma once

#include <Engine/Entity.hpp>
#include <Engine/System.hpp>
#include <Engine/AssetManager/AssetManager.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>

class Engine{
private:
    std::vector<Entity> entities;
    std::vector<std::unique_ptr<System>> systems;

    float time;
public:
    GLFWwindow* window;

    std::unique_ptr<AssetManager> assetManager;

    static std::unique_ptr<Engine> CreateEngineInstance(const int& width, const int& height, const std::string& title);

    template <class S>
    void AddSystem(){
        static_assert(std::is_base_of<System, S>(), "a classe S não é derivada de System");
        systems.push_back(std::make_unique<S>(this));
        systems.back()->onAwake();
    }

    Entity& CreateEntity();
    void DestroyEntity(Entity&);

    void run();
};