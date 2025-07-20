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

    if (auto rigidbody = get_component<RigidBody>()) {

        btTransform transform;
        rigidbody->get_body()->getMotionState()->getWorldTransform(transform);
        btQuaternion rotation = transform.getRotation();

        btScalar matrix[16];
        transform.getOpenGLMatrix(matrix);

        glMultMatrixf(matrix);

        auto t = get_component<Transform>();
        glScalef(t->scale.x,
            t->scale.y,
            t->scale.z);

    }
    else if (auto transform = get_component<Transform>()) {
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

    for (auto& ch : children) {
        ch->render();
    }

    glPopMatrix();
}
