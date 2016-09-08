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
	void registerNewPhysicsObject(VRGPhysicsObject obj, bool isMoveable);
	// void registerNewMoveableObject(VRGPhysicsObject obj, bool isMoveable);
	void calculateNewTick();
	void calculateNewTickForPhysicsObject(VRGPhysicsObject& obj);

	bool collision(VRGPhysicsObject& obj1, VRGPhysicsObject& obj2);
	bool distance(VRGPhysicsObject& obj1, VRGPhysicsObject& obj2);

	bool didHitPLattform(VRGPhysicsObject& obj);

	Vec3f getReflectionVector();

private:
	PhysicsModel model;
	int tick;
	float time;
	clock_t startTime;

	int heightDimension;
	double floorValue;
	float speed;
	float speedloss;
	float gravity;
	float minDirectionLengthValue;
	Vec3f upVector;

	Vec3f reflectionVector;

	Vec3f PhysicsController::calcReflectionVector(Vec3f direction, Vec3f normal);
	//std::thread physicThread1;
	// Thread* threadOne = dynamic_cast<Thread *>(ThreadManager::the()->getThread("One"));

};

