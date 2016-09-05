#include "VRGPhysicsObject.h"


VRGPhysicsObject::VRGPhysicsObject(void){
	direction = new Vec3f(0,0,0);
}

VRGPhysicsObject::~VRGPhysicsObject(void)
{

}

Vec3f VRGPhysicsObject::getDirection(){
	return *direction;
}

float VRGPhysicsObject::getSpeed(){
	return speed;
}

void VRGPhysicsObject::setDirection(Vec3f newDirection){
	direction->setValues(newDirection[0],newDirection[1],newDirection[2]);
}

void VRGPhysicsObject::setDirection(float x, float y, float z){
	direction->setValues(x,y,z);
}

void VRGPhysicsObject::setSpeed(float newSpeed){

}