#include "GameObject.h"

void GameObject::update(float delta_time)
{
	for (auto& c : components) {
		c->update(delta_time);
	}
}

void GameObject::render()
{
	if (!is_active) return;

	glPushMatrix();

	Transform* transform = get_component<Transform>();
	if (transform != nullptr) {
		glTranslatef(transform->position.x, transform->position.y, transform->position.z);
		glm::mat4 rotation_matrix = glm::mat4(transform->rotation);
		glMultMatrixf(glm::value_ptr(rotation_matrix));
		glScalef(transform->scale.x, transform->scale.y, transform->scale.z);
	}
	for (auto& c : components) {
		c->render();
	}

	glPopMatrix();
}
