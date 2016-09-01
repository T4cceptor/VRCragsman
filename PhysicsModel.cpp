#include "PhysicsModel.h"


PhysicsModel::PhysicsModel(void)
{
	dynamicObjects = * new std::list<VRGPhysicsObject>();
	staticObjects = * new std::list<VRGPhysicsObject>();
}


PhysicsModel::~PhysicsModel(void)
{
}

void PhysicsModel::addMoveableObject(VRGPhysicsObject newObject){
	dynamicObjects.push_front(newObject);
}

void PhysicsModel::deleteMoveableObject(int id){

}

std::list<VRGPhysicsObject> PhysicsModel::getMovableItems(){
	return dynamicObjects;
}
std::list<VRGPhysicsObject> PhysicsModel::getStaticItems(){
	return staticObjects;
}
VRGPhysicsObject PhysicsModel::getMoveableItemById(int id){
	// TODO
	// evtl. ist diese Methode überflüssig

	VRGPhysicsObject result;
	//for (std::list<VRGPhysicsObject>::iterator it = dynamicObjects.begin(); it != dynamicObjects.end(); it++){
	//	
	//}
	return result;
}
VRGPhysicsObject PhysicsModel::getStaticItemById(int id){
	// TODO
	// evtl. ist diese Methode überflüssig
	VRGPhysicsObject result;
	//for (std::list<VRGPhysicsObject>::iterator it = staticObjects.begin(); it != dynamicObjects.end(); it++){
	//	
	//}
	return result;
}