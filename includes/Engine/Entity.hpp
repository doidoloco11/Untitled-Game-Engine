#pragma once

#include <Engine/Component.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>

class Entity{
private:
    std::unordered_map<void(*)(), std::unique_ptr<Component>> components;

    template <typename T>
    static void hashCode(){};
public:
    bool willBeDeleted = false;

    uint64_t uuid;

    Entity();

    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    template <typename C>
    C* getComponent(){
        static_assert(std::is_base_of<Component, C>(), "a struct C não é derivada de Component");
        auto it = components.find(hashCode<C>);
        if (it != components.end() && !it->second->isDeleted()){
            return static_cast<C*>(it->second.get());
        }
        return nullptr;
    }
    template <typename C>
    C* addComponent(){
        static_assert(std::is_base_of<Component, C>(), "a struct C não é derivada de Component");
        components[hashCode<C>] = std::make_unique<C>();
        return static_cast<C*>(components[hashCode<C>].get());
    }
};