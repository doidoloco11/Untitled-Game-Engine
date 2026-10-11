#pragma once

#include <Engine/Entity.hpp>

class Engine;

#include <vector>

class System{
private:
protected:
    Engine* engine;
public:
    System(Engine* engine) : engine(engine) {}
    virtual void onAwake() {};
    virtual void onResizeWindow(std::vector<Entity>&, const int& width, const int& height) {};
    
    virtual void onStart(std::vector<Entity>&) {};
    virtual void onUpdate(std::vector<Entity>&, const float& deltaTime) {};
    virtual void onRender(std::vector<Entity>&) {};
    virtual void onTerminate(std::vector<Entity>&) {};
};