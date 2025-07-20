#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glut.h>
#include <algorithm>

#include "Transform.h"

#include "CubeRenderer.h"
#include "SphereRenderer.h"
#include "ConeRenderer.h"
#include "MeshRenderer.h"

#include "RigidBody.h"

class GameObject
{
public:
	std::string name;
	bool is_active = true;

    std::vector<std::unique_ptr<Component>> components;

    GameObject* parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> children;

    GameObject(const std::string& n = "GameObject") : name(n) {
        add_component<Transform>();
    }

    ~GameObject(){}

    void update(float delta_time);
    void render();

    /**
    * Creates a new child GameObject, assigns it this GameObject as its parent,
    * and adds it to the list of children.
    *
    * This method is used when you want to instantiate a child directly
    * by providing its name. The method returns a raw pointer to the new child.
    *
    * @param child_name The name of the new child GameObject.
    * @return A raw pointer to the newly created child GameObject.
    */
    GameObject* add_child(const std::string& child_name) {
        auto child = std::make_unique<GameObject>(child_name);
        child->parent = this;
        GameObject* ptr = child.get();
        children.push_back(std::move(child));
        return ptr;
    }

    /**
     * Adds an existing GameObject as a child to this GameObject.
     *
     * This method takes ownership of the passed-in unique_ptr,
     * sets the child's parent pointer to this GameObject,
     * and stores it in the list of children.
     *
     * @param child A unique pointer to a GameObject to be added as a child.
     */
    void add_child(std::unique_ptr<GameObject> child) {
        child->parent = this;
        children.push_back(std::move(child));
    }

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
        if (std::is_same<T, Transform>::value)
            return false;
            
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
        static_cast<Component*>(ptr)->init();
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

