#pragma once
#include <transform.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 forward;
    glm::vec3 up;

    Transform() :
        position(0.0f),
        rotation(1.0f, 0.0f, 0.0f),
        scale(1.0f),
        forward(1.0f, 0.0f, 0.0f),
        up(0.0f, 1.0f, 0.0f) {}
};

class Component
{
public:
    virtual ~Component() = default;
};

class Entity
{
private:
    using EntityID = size_t;
    static EntityID nextId;
    EntityID id;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
public:
    
	Transform transform;

    Entity() : id(nextId++) {}

	int GetID() const { return id; }

    // Add a component to this entity
    template <typename T, typename... Args>
    void AddComponent(Args&&... args)
    {
        components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    // Get a component attached to this entity
    template <typename T>
    T* GetComponent()
    {
        auto it = components.find(typeid(T));
        if (it != components.end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    // Check if the entity has a specific component
    template <typename T>
    bool HasComponent() const
    {
        return components.find(typeid(T)) != components.end();
    }
};