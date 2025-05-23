#pragma once

class GameObject;

#include "glm.hpp"
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

class Component {
public:
	GameObject* owner = nullptr;
	virtual ~Component() = default;
	virtual void update(float delta_time){}
	virtual void render(){}
};

class Transform : public Component {
public:
	glm::vec3 position = { 0, 0, 0 };
	glm::quat rotation = { 0, 0, 0, 1 };
	glm::vec3 scale = { 1, 1, 1 };
};

class MeshRenderer : public Component {
public:
	virtual void render() override = 0;
};