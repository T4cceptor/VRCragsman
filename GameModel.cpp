#include "GameModel.h"
#include <math.h>
#include "Config.h"


GameModel::GameModel(void)
{
	std::cout << "GameModel contructor called" << std::endl;
}

/* TODO Parameter für Start übergeben - ?
GameModel::GameModel(void)
{

}
*/

GameModel::~GameModel(void)
{
}

/** GETTER **/
VRGObject GameModel::getScenegraphRoot(){
	return root;
}

VRGPhysicsObject GameModel::getCave(){
	return cave;
}

std::list<PlattformObject> GameModel::getPlattforms(){
	return plattforms;
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
std::list<VRGPhysicsObject> GameModel::getRopeTail(){
	return ropePieces;
}

/* // TODO
VRGPhysicsObject GameModel::getRopePieceWithId(){
	VRGPhysicsObject result;
	for (std::list<VRGPhysicsObject>::iterator it = ropePieces.begin(); it != ropePieces.end(); it++){
		if(it->)
	}
	return 
}
*/

std::list<VRGPhysicsObject> GameModel::getIcicles(){
	return icicles;
}

/* // TODO
VRGPhysicsObject GameModel::getIcicleWithId(){

}
*/

/* // TODO
PlattformObject GameModel::getPlattformWithId(){

}
*/

/** Initialize Model and create Scenegraph functions **/
void GameModel::initGameModel(PhysicsController * pc){
	nf = * new NodeFactory();
	physicCtrl = *pc;
	// std::thread physicThread1(&physicCtrl.calculateNewTick, physicCtrl);
	// physicThread1.join();

	createScenegraph();
}


extern void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

VRGPhysicsObject GameModel::createNewHook(Pnt3f pos, Vec3f direction){
	VRGPhysicsObject newHook = nf.createHook();
	ComponentTransformRecPtr t = newHook.getTransformation();
	t->setScale(hook::scaleVector);
	t->setTranslation(pos.subZero());

	newHook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);

	// Der Haken wird anhand der aktuellen Blickrichtung ausgerichtet
	// TODO für Cave -> MagicWand Vector
	MatrixLookAt(pos, pos + direction, Vec3f(0,0,1), t->editRotation());

	//lightedScene.addChild(newHook);
	physicCtrl.registerNewPhysicsObject(newHook, true);
	return newHook;
}

void GameModel::moveHook(Pnt3f pos, Vec3f direction){
	// Der Haken wird anhand der aktuellen Blickrichtung ausgerichtet
	// TODO: Haken anhand des MWand Vektors ausrichten
	// TODO für Cave -> MagicWand Vector
	
	
	hook.setPosition(pos[0],pos[1],pos[2]);
	hook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);
	hook.setLookAt(direction);
	// MatrixLookAt(pos, pos + direction, Vec3f(0,1,0), hook.getRotation());
	std::cout << "position: " << pos << std::endl;
	std::cout << "direction: " << direction << std::endl;
	std::cout << "rotation: " << hook.getRotation() << std::endl;
}

// TODO
void GameModel::createNewLight(Pnt3f pos){
	/*
	DirectionalLightRecPtr dirLight = DirectionalLight::create();
	dirLight->setDirection(pos.x,pos.y,pos.z);

	//color information
	dirLight->setDiffuse(Color4f(1,1,1,1));
	dirLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	dirLight->setSpecular(Color4f(1,1,1,1));


	//wrap the root, cause only nodes below the lights will be lit
	NodeRecPtr ueberroot = makeNodeFor(dirLight);
	ueberroot->addChild(root);
	
	NodeRecPtr newHook = (NodeRecPtr)nf.createNewLight(pos);
	//newHook->getChild(1);

	//t->setTranslation(Vec3f(15,0,15));
	VRGLogger::logMessage(SSTR( "x: " << pos[0] << " y: " << pos[1] << " z: " << pos[2]));

	root.addChild(newHook);
	*/
}

void GameModel::rotateCave(float x, float y, float z, float rad){
	ComponentTransformRecPtr newTransformation = cave.getTransformation();
	newTransformation->setRotation(Quaternion(Vec3f(x,y,z),osgDegree2Rad(rad)));
}

void GameModel::createScenegraph(){
	root = nf.createRoot();

	// Lights preparation
	lightTopPtr = Node::create();
	lightTopPtr->setCore(Group::create());
	staticCaveLights = * new std::list<NodeRecPtr>();
	lightedScene = nf.createEmptyVRGObj();

	// TODO setup light 
	NodeRecPtr testLight = (NodeRecPtr)nf.createNewLight(configLight::positions[0], root.getRootNode());
	staticCaveLights.push_front(testLight);
	// Idee: 
	// lights = nf.createLightSetup();
	// staticCaveLights = lights;
	for (std::list<NodeRecPtr>::iterator it=staticCaveLights.begin(); it != staticCaveLights.end(); ++it){
		(* it)->addChild(lightedScene.getRootNode());
	}
	//root.addChild(testLight);
	// testLight->addChild(lightedScene.getRootNode());
	//root.addChild(lightTopPtr);

	// TODO init cave
	cave = nf.createCave();
	lightedScene.addChild(cave.getRootNode());

	// TODO init hook
	hook = createNewHook(Pnt3f(0,0,0), Vec3f(0,-1,0));
	lightedScene.addChild(hook);

	// TODO init plattforms

	// Plattform1Only

	// TODO init camera ????
	// TODO init player ????

	// TODO init rope

	// TODO init icicles
	
}