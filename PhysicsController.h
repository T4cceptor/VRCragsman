#pragma once
#include "PhysicsModel.h"
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
	//std::thread physicThread1;
	// Thread* threadOne = dynamic_cast<Thread *>(ThreadManager::the()->getThread("One"));

};

