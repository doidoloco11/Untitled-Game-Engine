#pragma once

#include <Engine/Entity.hpp>
#include <Engine/System.hpp>
#include <Engine/AssetManager/AssetManager.hpp>
#include <Engine/SceneManager.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>

namespace uge{
    class Engine{
    private:
        
        std::vector<std::unique_ptr<System>> systems;
        std::vector<Entity> entities;

        float time;
    public:
        GLFWwindow* window;

        std::unique_ptr<AssetManager> assetManager;
        std::unique_ptr<SceneManager> sceneManager;

        Engine(const int& width, const int& height, const std::string& title);
        ~Engine();

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
}