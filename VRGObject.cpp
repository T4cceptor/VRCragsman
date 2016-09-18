#include "VRGObject.h"
#include <iostream>


VRGObject::VRGObject(void)
{
}


VRGObject::~VRGObject(void)
{
}

void VRGObject::setGeometryNode(OSG::NodeRecPtr newGeoNode){
	geoNode = newGeoNode;
}

void VRGObject::setTransformation(OSG::ComponentTransformRecPtr newTransform){
	objectTransformation = newTransform;
}

Quaternion VRGObject::getRotation(){
	return objectTransformation->getRotation();
}

Vec3f VRGObject::getPosition(){
	return objectTransformation->getTranslation();
}

void VRGObject::setPosition(float x, float y, float z){
	return objectTransformation->setTranslation(Vec3f(x,y,z));
}

void VRGObject::setPosition(Vec3f newPosition){
	setPosition(newPosition[0], newPosition[1], newPosition[2]);
}

void VRGObject::addTranslation(Vec3f addVec){
	objectTransformation->setTranslation(getPosition() + addVec);
}

ComponentTransformTransitPtr VRGObject::getTransformation(){
	return (ComponentTransformTransitPtr) objectTransformation;
}

NodeTransitPtr VRGObject::getGeoNode(){
	return (NodeTransitPtr) geoNode;
}

void VRGObject::setRootNode(NodeRecPtr newRoot){
	root = newRoot;
}

NodeTransitPtr VRGObject::getRootNode(){
	return (NodeTransitPtr) root;
}

void VRGObject::addChild(NodeRecPtr child){
	root->addChild(child);
}

void VRGObject::addChild(VRGObject obj){
	root->addChild(obj.getRootNode());
}