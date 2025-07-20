#pragma once
#include "Component.h"
#include "Transform.h"

#include "CubeRenderer.h"
#include "SphereRenderer.h"
#include "ConeRenderer.h"

#include <string>
#include <memory>
#include <vector>
#include <glut.h>

class GameObject
{
public:
	std::string name;
	bool is_active = true;

    std::vector<std::unique_ptr<Component>> components;

    GameObject(const std::string& n = "GameObject") : name(n) {
        add_component<Transform>();
    }

    ~GameObject(){}

    void update(float delta_time);
    void render();

    /**
     * Removes the first component of type T attached to this GameObject.
     *
     * This method scans the `components` vector for the first element whose
     * raw pointer can be dynamically cast to T*. If found, it erases that
     * unique_ptr from the vector and returns true. If no component of type T
     * exists, returns false.
     *
     * @tparam T The type of the component to remove (must derive from Component).
     * @return   True if a component was found and removed; false otherwise.
     */
    template<typename T>
    bool remove_component() {
        auto it = std::find_if(components.begin(), components.end(),
            [](const std::unique_ptr<Component>& c) {
                return dynamic_cast<T*>(c.get()) != nullptr;
            });
        if (it != components.end()) {
            components.erase(it);
            return true;
        }
        return false;
    }

    /**
    * Adds a new component of type T to the GameObject.
    *
    * This method constructs the component with the given arguments,
    * sets its owner to this GameObject, stores it internally,
    * and returns a raw pointer to the component.
    *
    * @tparam T The type of the component to add (must derive from Component).
    * @tparam Args Variadic template for constructor arguments of T.
    * @param args Arguments forwarded to the component constructor.
    * @return A raw pointer to the newly created component.
    */
    template<typename T, typename... Args>
    T* add_component(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->owner = this;
        T* ptr = component.get();
        components.push_back(std::move(component));
        return ptr;
    }

    /**
     * Retrieves the first component of type T attached to this GameObject.
     *
     * This method searches through all components and returns the first one
     * that can be dynamically cast to type T. If no such component exists,
     * returns nullptr.
     *
     * @tparam T The type of component to retrieve.
     * @return Pointer to the component if found, nullptr otherwise.
     */
    template<typename T>
    T* get_component() {
        for (auto& c : components)
            if (auto t = dynamic_cast<T*>(c.get())) return t;
        return nullptr;
    }
};

