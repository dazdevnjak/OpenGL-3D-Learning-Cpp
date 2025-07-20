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

    if (auto transform = get_component<Transform>()) {
        glTranslatef(transform->position.x,
            transform->position.y,
            transform->position.z);

        glm::mat4 rotMat = glm::mat4_cast(transform->rotation);
        glMultMatrixf(glm::value_ptr(rotMat));

        glScalef(transform->scale.x,
            transform->scale.y,
            transform->scale.z);
    }

    for (auto& c : components) {
        c->render();
    }

    glPopMatrix();
}
