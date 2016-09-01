#pragma once
#include "VRGPhysicsObject.h"
#include <list>;

class PhysicsModel
{
public:
	PhysicsModel(void);
	~PhysicsModel(void);

	std::list<VRGPhysicsObject> getMovableItems();
	std::list<VRGPhysicsObject> getStaticItems();
	VRGPhysicsObject getMoveableItemById(int id);
	VRGPhysicsObject getStaticItemById(int id);

	void addMoveableObject(VRGPhysicsObject newObject);
	void deleteMoveableObject(int id);

private:
	std::list<VRGPhysicsObject> dynamicObjects;
	std::list<VRGPhysicsObject> staticObjects;
};

