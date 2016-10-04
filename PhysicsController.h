#pragma once
#include "OSGIntersectAction.h"
#include "VRGPhysicsObject.h"

OSG_USING_NAMESPACE

class PhysicsController
{
public:
	PhysicsController(void);
	~PhysicsController(void);

	void calculateNewTickForPhysicsObject(VRGPhysicsObject obj);
	bool collision(VRGPhysicsObject obj1, VRGObject obj2);
	bool distance(VRGPhysicsObject& obj1, VRGPhysicsObject& obj2);

	int didHitPlatform(VRGPhysicsObject obj, VRGObject pltformRoot);

	Vec4f overthrow(Line line, VRGObject obj, float length);

	Vec3f getReflectionVector();
	Vec3f calcReflectionVector(Vec3f direction, Vec3f normal);
private:
	// const values
	int heightDimension;
	double floorValue;
	float speed;
	float speedloss;
	float gravity;
	float minDirectionLengthValue;
	Vec3f upVector;

	Vec3f reflectionVector;
};

