#include "GameModel.h"
#include <math.h>
#include "Config.h"

GameModel::GameModel(void){ std::cout << "GameModel contructor called" << std::endl; }
GameModel::~GameModel(void){}

extern void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation);

/** GETTER **/
VRGObject GameModel::getScenegraphRoot(){
	return root;
}

VRGObject GameModel::getCave(){
	return caveRoot;
}

VRGObject GameModel::getPltformRoot(){
	return platformRoot;
}

VRGPhysicsObject GameModel::getPlayer(){
	return player;
}
VRGPhysicsObject GameModel::getHook(){
	return hook;
}
VRGPhysicsObject GameModel::getRopeStart(){
	return ropeStart;
}

VRGPhysicsObject GameModel::getAnchor(){
	return anchor;
}

std::list<VRGPhysicsObject> GameModel::getRopeTail(){
	return ropePieces;
}

std::list<VRGPhysicsObject> GameModel::getIcicles(){
	return icicles;
}

/** Initialize Model and create Scenegraph functions **/
void GameModel::initGameModel(PhysicsController * pc){
	nf = * new NodeFactory();
	nf.initNodeFactory();
	physicCtrl = *pc;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

VRGPhysicsObject GameModel::createNewHook(Pnt3f pos, Vec3f direction){
	VRGPhysicsObject newHook = nf.createHook();
	ComponentTransformRecPtr t = newHook.getTransformation();
	t->setScale(hook::scaleVector * general::scale);
	t->setTranslation(pos.subZero());
	newHook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);
	MatrixLookAt(pos, pos + direction, Vec3f(0,0,1), t->editRotation());
	return newHook;
}

void GameModel::moveHook(Pnt3f pos, Vec3f direction){
	hook.setPosition(pos[0],pos[1],pos[2]);
	hook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);
	hook.setLookAt(direction);
	std::cout << "position: " << pos << std::endl;
	std::cout << "direction: " << direction << std::endl;
	std::cout << "rotation: " << hook.getRotation() << std::endl;
}

void GameModel::setupLights(){	
	int count = configLight::positionsSize;
	NodeRecPtr lightNode = root.getRootNode();
	for(int i = 0; i < count; i++){
		NodeRecPtr newLightNode = (NodeRecPtr)nf.createNewLight(configLight::positions[i] * general::scale, lightNode);
		PointLightRecPtr l2 =  dynamic_cast<PointLight*>(newLightNode->getCore());
		Vec3f attenuation = configLight::attenuations[i];
		Vec4f diffuseColor = configLight::diffuseColors[i];
		Vec4f ambientColor = configLight::ambientColors[i];
		Vec4f specularColor = configLight::specularColors[i];
		if(attenuation.length() > 0)
			l2->setAttenuation(attenuation[0], attenuation[1], attenuation[2]);
		if(ambientColor.length() > 0)
			l2->setAmbient(ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3]);
		if(diffuseColor.length() > 0)
			l2->setDiffuse(diffuseColor[0], diffuseColor[1], diffuseColor[2], diffuseColor[3]);
		if(specularColor.length() > 0)
			l2->setSpecular(specularColor[0], specularColor[1], specularColor[2], specularColor[3]);

		if(i == count - 1)
			newLightNode->addChild(lightedScene.getRootNode());
		lightNode = newLightNode;
	}
}

void GameModel::setupIcicles(){
	int count = icicles::positionsSize;
	VRGObject rNode = caveRoot;
	for(int i = 0; i < count; i++){
		VRGPhysicsObject newIcicle = nf.createNewIcicle(icicles::positions[i], 1);
		rNode.addChild(newIcicle);
	}
}

void GameModel::setupStalactites(){
	int count = stalactites::positionsSize;
	VRGObject rNode = caveRoot;
	for(int i = 0; i < count; i++){
		VRGPhysicsObject newStalac = nf.createStalactites(stalactites::positions[i]);
		rNode.addChild(newStalac);
	}
}

void GameModel::setupSpecialObjs(){
	int count = specialObjs::count;
	VRGObject rNode = caveRoot;
	for(int i = 0; i < count; i++){
		VRGPhysicsObject newStalac = nf.createObjectFromPath(specialObjs::paths[i], Vec3f(1,1,1), specialObjs::scaleVector * general::scale);
		rNode.addChild(newStalac);
	}
}

void GameModel::setupPltforms(){
	int count = platforms::count;
	VRGObject rNode = platformRoot;
	for(int i = 0; i < count; i++){
		VRGPhysicsObject newStalac = nf.createObjectFromPath(platforms::paths[i], Vec3f(0,0,0), platforms::scaleVector * general::scale);
		rNode.addChild(newStalac);
	}
	caveRoot.addChild(platformRoot);
}

void GameModel::setupPebbleGroup1(){
	int count = pebble::positionsSize;
	VRGObject rNode = caveRoot;
	for(int i = 0; i < count; i++){
		VRGPhysicsObject newStalac = nf.createPebble(pebble::positions[i]);
		rNode.addChild(newStalac);
	}

	count = pebble::positionsSize2;
	for(int i = 0; i < count; i++){
		int randNum = rand()%(360) + 1;
		std::cout << "random rotation: " << randNum << "\n";
		Quaternion rotation = Quaternion(Vec3f(0,1,0), osgDegree2Rad(randNum));
		VRGPhysicsObject newStalac = nf.cloneObjectFromNode(
			nf.pebble2Blueprint, 
			pebble::positions2[i] * general::scale, 
			pebble::scaleVector * general::scale * 0.8,
			rotation
			);
		rNode.addChild(newStalac);
	}
}

void GameModel::setupAdditionalMeshes(){
	setupStalactites();
	// setupIcicles();
	setupPebbleGroup1();
	setupSpecialObjs();

	// random number: int randNum = rand()%(max-min + 1) + min;
}

void GameModel::createScenegraph(){
	// scenegraph root	
	root = nf.createRoot();

	// Lights preparation
	lightTopPtr = Node::create();
	lightTopPtr->setCore(Group::create());
	staticCaveLights = * new std::list<NodeRecPtr>();
	lightedScene = nf.createEmptyVRGObj();
	platformRoot = nf.createEmptyVRGObj();
	caveRoot = nf.createEmptyVRGObj();
	lightedScene.addChild(caveRoot);

	// light setup
	setupLights();

	cave = nf.createCave();
	caveRoot.addChild(cave.getRootNode());
	setupAdditionalMeshes();
	setupPltforms();

	hook = createNewHook(Pnt3f(0,0,0), Vec3f(0,-1,0));
	lightedScene.addChild(hook);

	for(int i = 0; i < rope::pieces; i++){
		ropePieces.push_back(nf.createRopePiece());
	}
	for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
		lightedScene.addChild((* it).getRootNode());
	}

	anchor = nf.createAnchor();
	lightedScene.addChild(anchor);
}

void GameModel::createRopePiece(){
	 
}