#pragma once

#include "btBulletDynamicsCommon.h"
#include "MotionStateObject.h"
#include "PhysicsUtilities.h"
#include "Component.h"
#include "Transform.h"

class GameObject;

class RigidBody : public Component {
public:
    RigidBody(float mass_amount,
        btCollisionShape* collider_shape,
        btDynamicsWorld* bt_world_n);

    ~RigidBody();

    btRigidBody* get_body() const { return rigid_body; }

    void update(float delta_time) override;

    void init();

private:
    MotionStateObject* motion_state;
    btCollisionShape* collision_shape;
    btRigidBody* rigid_body;
    btDynamicsWorld* bt_world;
    float mass;
};
