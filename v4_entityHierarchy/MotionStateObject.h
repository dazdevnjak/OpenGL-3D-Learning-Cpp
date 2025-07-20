#ifndef _OPENGLMOTIONSSTATE_H_
#define _OPENGLMOTIONSSTATE_H_
#include "btBulletCollisionCommon.h"

class MotionStateObject : public btDefaultMotionState
{
public:
	MotionStateObject(const btTransform& transform) : btDefaultMotionState(transform) {}

	void GetWorldTransform(btScalar* transform) {
		btTransform trans;
		getWorldTransform(trans);
		trans.getOpenGLMatrix(transform);
	}

	~MotionStateObject() {};
};
#endif // !_OPENGLMOTIONSSTATE_H_