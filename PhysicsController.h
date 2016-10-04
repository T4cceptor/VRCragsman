#pragma once
#include "PhysicsModel.h"
#include "OSGIntersectAction.h"
#include <thread>

OSG_USING_NAMESPACE // activate the OpenSG namespace

class PhysicsController
{
public:
	PhysicsController(void);
	~PhysicsController(void);

	// deprecated - use calculateNewTickForPhysicsObject instead
	// void registerNewPhysicsObject(VRGPhysicsObject obj, bool isMoveable);
	// void registerNewMoveableObject(VRGPhysicsObject obj, bool isMoveable);
	// void calculateNewTick();

	void calculateNewTickForPhysicsObject(VRGPhysicsObject obj);

	bool collision(VRGPhysicsObject obj1, VRGObject obj2);
	bool distance(VRGPhysicsObject& obj1, VRGPhysicsObject& obj2);

	int didHitPlatform(VRGPhysicsObject obj, VRGObject pltformRoot);

	Vec4f overthrow(Line line, VRGObject obj, float length);
	bool pointInsideObj(Vec3f point, VRGPhysicsObject obj);

	Vec3f getReflectionVector();
	Vec3f calcReflectionVector(Vec3f direction, Vec3f normal);
private:
	PhysicsModel model;
	int tick;
	float time;
	clock_t startTime;

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

