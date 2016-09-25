#include "VRGPhysicsObject.h"
#include "Config.h"

VRGPhysicsObject::VRGPhysicsObject(void){
	direction = new Vec3f(0,0,0);
	lookAt = new Vec3f(0,0,0);
}

VRGPhysicsObject::~VRGPhysicsObject(void)
{

}

Vec3f VRGPhysicsObject::getDirection(){
	return *direction;
}

Vec3f VRGPhysicsObject::getLookAt(){
	return *lookAt;
}

float VRGPhysicsObject::getSpeed(){
	return speed;
}

Vec3f VRGPhysicsObject::getOffsetPosition(){
	Vec3f offset = *lookAt;
	offset.normalize();
	return getPosition() - offset * hook::lookAtOffset;
}

void VRGPhysicsObject::setLookAt(Vec3f newDirection){
	lookAt->setValues(newDirection[0],newDirection[1],newDirection[2]);
}

void VRGPhysicsObject::setDirection(Vec3f newDirection){
	direction->setValues(newDirection[0],newDirection[1],newDirection[2]);
}

void VRGPhysicsObject::setDirection(float x, float y, float z){
	direction->setValues(x,y,z);
}

void VRGPhysicsObject::setSpeed(float newSpeed){
	
}