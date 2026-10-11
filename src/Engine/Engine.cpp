#include <Engine/Engine.hpp>
#include <Engine/Entity.hpp>
#include <Engine/AssetManager/AssetManager.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>

std::unique_ptr<Engine> Engine::CreateEngineInstance(const int& width, const int& height, const std::string& title){
    std::unique_ptr<Engine> engine = std::make_unique<Engine>();
    engine->assetManager = std::make_unique<AssetManager>();
    
    if (glfwInit() == GLFW_FALSE) throw std::runtime_error("glfwInit falhou");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    engine->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (engine->window == NULL) throw std::runtime_error("glfwCreateWindow falhou");

    glfwMakeContextCurrent(engine->window);

    glfwSetWindowUserPointer(engine->window, engine.get());

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glfwSetFramebufferSizeCallback(engine->window, [](GLFWwindow* window, const int width, const int height) -> void{
        glViewport(0, 0, width, height);
        Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
        if (engine){
            for (int i = 0; i < engine->systems.size(); i++) {
                engine->systems[i]->onResizeWindow(engine->entities, width, height);
            }
        }
    });

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS);
    
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return engine;
}

Entity& Engine::CreateEntity(){
    entities.push_back(Entity{});
    return entities.back();
}

void Engine::DestroyEntity(Entity& entity){
    entity.willBeDeleted = true;
}

void Engine::run(){
    time = glfwGetTime();
    for (int i = 0; i < systems.size(); i++){
        systems[i]->onStart(entities);
    }
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        float newTime = glfwGetTime();
        float deltaTime = newTime - time;
        time = newTime;

        for (int i = 0; i < systems.size(); i++){
            systems[i]->onUpdate(entities, deltaTime);
        }

        for (int i = 0; i < systems.size(); i++){
            systems[i]->onRender(entities);
        }
        
        entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](Entity& entity) -> bool {return entity.willBeDeleted;}), 
        entities.end());

        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    for (int i = 0; i < systems.size(); i++){
        systems[i]->onTerminate(entities);
    }
    entities.clear();

    glfwDestroyWindow(window);
    glfwTerminate();
}