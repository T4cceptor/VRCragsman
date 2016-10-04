#include "NodeFactory.h"
#include "Config.h"

NodeFactory::NodeFactory(void){}
NodeFactory::~NodeFactory(void){}

NodeTransitPtr createNodeFromFile(NodeRecPtr root, std::string filePath){
	NodeRecPtr cave = SceneFileHandler::the()->read(filePath.c_str());
	root->addChild(cave);
	return NodeTransitPtr(root);
}

NodeTransitPtr createNodeFromFile(std::string filePath){
	NodeRecPtr cave = SceneFileHandler::the()->read(filePath.c_str());
	return NodeTransitPtr(cave);
}

void NodeFactory::initNodeFactory(){
	ropeBlueprint = createNodeFromFile(path::ropeModel);
	icicleBlueprint = createNodeFromFile(path::icicleModel);
	stalactitesBlueprint = createNodeFromFile(path::stalactitesModel);
	pebbleBlueprint = createNodeFromFile(path::pebbleModel);
	pebble2Blueprint = createNodeFromFile(path::pebbleModel2);
}

VRGPhysicsObject NodeFactory::createNewIcicle(Vec3f position, float scale){
	Vec3f newScale = scale * icicles::scaleVector * general::scale;
	Vec3f newPosition = position * general::scale;
	return cloneObjectFromNode(icicleBlueprint, newPosition, newScale);
}

VRGObject NodeFactory::createEmptyVRGObj(){
	VRGObject returnObj = * new VRGObject();
	NodeRecPtr root = Node::create();
	root->setCore(Group::create());
	returnObj.setRootNode(root);
	return returnObj;
}

VRGPhysicsObject NodeFactory::createRoot(){
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();
	NodeRecPtr root = Node::create();
	root->setCore(Group::create());
	returnObj.setRootNode(root);
	return returnObj;
}

VRGPhysicsObject NodeFactory::createRopePiece(){
	Vec3f newScale = rope::scaleVector * general::scale;
	Vec3f newPosition = rope::translationVector * general::scale;
	return cloneObjectFromNode(ropeBlueprint, newPosition, newScale);
}

VRGPhysicsObject NodeFactory::createHook(){
	VRGPhysicsObject returnObj = createObjectFromPath(path::hookModel, Vec3f(0.0f), Vec3f(0.0f));
	return returnObj;
}

VRGPhysicsObject NodeFactory::createAnchor(){
	return createObjectFromPath(path::anchorModel, Vec3f(0,0,0), anchor::scaleVector);
}

VRGPhysicsObject NodeFactory::createStalactites(Vec3f position){
	Vec3f newScale = stalactites::scaleVector * general::scale;
	Vec3f newPosition = position * general::scale;
	return cloneObjectFromNode(stalactitesBlueprint, newPosition, newScale);
}

VRGPhysicsObject NodeFactory::createPebble(Vec3f position){
	Vec3f newScale = pebble::scaleVector * general::scale;
	Vec3f newPosition = position * general::scale;
	return cloneObjectFromNode(pebbleBlueprint, newPosition, newScale);
}

VRGPhysicsObject NodeFactory::createObject(std::string path){
	VRGPhysicsObject returnObj = createObjectFromPath(path, Vec3f(0,0,0), Vec3f(1,1,1));
	return returnObj;
}

VRGPhysicsObject NodeFactory::createCave(){
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();
	NodeRecPtr cave = createNodeFromFile(path::caveModel);
	returnObj.setGeometryNode(cave);
	GeometryRecPtr geo = dynamic_cast<Geometry *>(cave->getCore());
	// createSharedIndex(geo);
	// calcVertexNormals(geo);
    // calcVertexNormals(geo, osgDegree2Rad(30));
	// calcFaceNormals(geo);

	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);
	compTransform->setScale(cave::scaleVector * general::scale);
    compTransform->setTranslation(cave::translationVector * general::scale);
	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(cave);
	returnObj.setRootNode(transNode);
	return returnObj;
}

NodeTransitPtr NodeFactory::createNewLight(Pnt3f position, NodeRecPtr root){
	NodeRecPtr returnPtr = Node::create();
	returnPtr->setCore(Group::create());
	NodeRecPtr lightPosition = Node::create();
	ComponentTransformRecPtr ct = ComponentTransform::create();
	ct->setTranslation(position.subZero());
	lightPosition->setCore(ct);
	returnPtr->addChild(lightPosition);
	PointLightRecPtr sunLight = PointLight::create();
	//sunLight->setAttenuation(1,0,2);
	Color4f globalDiffuse = Color4f(0.6,0.6,0.6,1);
	Color4f globalAmbient = Color4f(0.2f,0.2f,0.2f,1);
	Color4f globalSpecular = Color4f(0.8,0.8,0.8,1);

	/*
	sunLight->setDiffuse(Color4f(1,1,1,1));
	sunLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	sunLight->setSpecular(Color4f(1,1,1,1));

	sunLight->setDiffuse(Color4f(0.2,0.3,0.5,1));
	sunLight->setAmbient(Color4f(0.3f,0.3f,0.3f,1));
	sunLight->setSpecular(Color4f(0.5,0.6,0.8,1));
	*/

	sunLight->setDiffuse(globalDiffuse);
	sunLight->setAmbient(globalAmbient);
	sunLight->setSpecular(globalSpecular);
	sunLight->setBeacon(lightPosition);
	NodeRecPtr sunLightNode = Node::create();
	sunLightNode->setCore(sunLight);
	returnPtr->addChild(sunLightNode);
	root->addChild(returnPtr);
	return NodeTransitPtr(sunLightNode);
}

VRGPhysicsObject NodeFactory::createObjectFromPath(std::string path, Vec3f position, Vec3f scale){
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();
	NodeRecPtr geoNode = createNodeFromFile(path);
	returnObj.setGeometryNode(geoNode);
	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);
	compTransform->setScale(scale);
	compTransform->setTranslation(position);
	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(geoNode);
	returnObj.setRootNode(transNode);
	return returnObj;
}

VRGPhysicsObject NodeFactory::cloneObjectFromNode(NodeRecPtr node, Vec3f position, Vec3f scale){
	VRGPhysicsObject returnObj = * new VRGPhysicsObject(); 
	NodeRecPtr geoNode = OSG::cloneTree(node);
	returnObj.setGeometryNode(geoNode);
	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);
	compTransform->setScale(scale);
	compTransform->setTranslation(position);
	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(geoNode);
	returnObj.setRootNode(transNode);
	return returnObj;
}

VRGPhysicsObject NodeFactory::cloneObjectFromNode(NodeRecPtr node, Vec3f position, Vec3f scale, Quaternion rotation){
	VRGPhysicsObject returnObj = cloneObjectFromNode(node, position, scale);
	returnObj.getTransformation()->setRotation(rotation);
	return returnObj;
}

// deprecated, platforms are integrated into the cave geometry
PlattformObject NodeFactory::createPlattform(std::string filePath){
	PlattformObject returnObj = * new PlattformObject();
	NodeRecPtr plattforms = createNodeFromFile(filePath);
	returnObj.setGeometryNode(plattforms);
	// Plattform1Only
	GeometryRecPtr geo = dynamic_cast<Geometry *>(plattforms->getCore());
	createSharedIndex(geo);
	// calcFaceNormals(geo);
	calcVertexNormals(geo);

	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);

	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(plattforms);
	returnObj.setRootNode(transNode);
	return returnObj;
}