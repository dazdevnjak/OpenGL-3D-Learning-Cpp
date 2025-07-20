#include "RigidBody.h"
#include "GameObject.h"  
#include "Transform.h" 

RigidBody::RigidBody(float mass_amount,
    btCollisionShape* collider_shape,
    btDynamicsWorld* bt_world_n)
    : mass(mass_amount),
    collision_shape(collider_shape),
    motion_state(nullptr),
    rigid_body(nullptr),
    bt_world(bt_world_n) {}

RigidBody::~RigidBody() {
    delete rigid_body;
    delete motion_state;
}

void RigidBody::update(float delta_time)
{
    auto t = owner->get_component<Transform>();

    btQuaternion bt_rot = rigid_body->getOrientation();
    glm::quat rotation = glm::quat(bt_rot.w(), bt_rot.x(), bt_rot.y(), bt_rot.z());

    t->position = PhysicsUtilities::bt_to_glm_vec3(&rigid_body->getWorldTransform().getOrigin());
    t->rotation = rotation;
}

void RigidBody::init() {
    auto t = owner->get_component<Transform>();
    btTransform start;
    start.setIdentity();
    start.setOrigin(PhysicsUtilities::glm_to_bt_vec3(t->position));

    motion_state = new MotionStateObject(start);

    btVector3 local_inertia(0, 0, 0);
    if (mass != 0.f)
        collision_shape->calculateLocalInertia(mass, local_inertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, collision_shape, local_inertia);
    rigid_body = new btRigidBody(info);

    rigid_body->setUserPointer(owner);

    bt_world->addRigidBody(rigid_body);
}
