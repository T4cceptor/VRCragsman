#pragma once
#include "VRGObject.h"
#include <OSGVector.h>

class VRGPhysicsObject :
	public VRGObject
{
public:
	VRGPhysicsObject(void);
	~VRGPhysicsObject(void);

	void setDirection(float x, float y, float z);
	void setDirection(Vec3f newDirection);
	void setLookAt(Vec3f newDirection);
	void setSpeed(float newSpeed);
	Vec3f getDirection();
	Vec3f getLookAt();
	Vec3f getOffsetPosition();

private:
	Vec3f * direction;
	Vec3f * lookAt;
};

