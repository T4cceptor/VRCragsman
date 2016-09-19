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
	nf.initNodeFactory();
	physicCtrl = *pc;
}


extern void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

VRGPhysicsObject GameModel::createNewHook(Pnt3f pos, Vec3f direction){
	VRGPhysicsObject newHook = nf.createHook();
	ComponentTransformRecPtr t = newHook.getTransformation();
	t->setScale(hook::scaleVector * general::scale);
	t->setTranslation(pos.subZero());

	newHook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);

	// Der Haken wird anhand der aktuellen Blickrichtung ausgerichtet
	// TODO für Cave -> MagicWand Vector
	MatrixLookAt(pos, pos + direction, Vec3f(0,0,1), t->editRotation());

	//lightedScene.addChild(newHook);
	// physicCtrl.registerNewPhysicsObject(newHook, true);
	return newHook;
}

void GameModel::moveHook(Pnt3f pos, Vec3f direction){
	// Der Haken wird anhand der aktuellen Blickrichtung ausgerichtet
	
	hook.setPosition(pos[0],pos[1],pos[2]);
	hook.setDirection(direction.getValues()[0], direction.getValues()[1], direction.getValues()[2]);
	hook.setLookAt(direction);
	// MatrixLookAt(pos, pos + direction, Vec3f(0,1,0), hook.getRotation());
	std::cout << "position: " << pos << std::endl;
	std::cout << "direction: " << direction << std::endl;
	std::cout << "rotation: " << hook.getRotation() << std::endl;
}

void GameModel::createScenegraph(){
	// scenegraph root	
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
	// staticCaveLights = nf.createLightSetup();
	for (std::list<NodeRecPtr>::iterator it=staticCaveLights.begin(); it != staticCaveLights.end(); ++it){
		(* it)->addChild(lightedScene.getRootNode());
	}

	cave = nf.createCave();
	lightedScene.addChild(cave.getRootNode());

	hook = createNewHook(Pnt3f(0,0,0), Vec3f(0,-1,0));
	lightedScene.addChild(hook);

	// TODO init rope
	// createDeepCopy !!
	ropeStart = nf.createRopePiece();
	lightedScene.addChild(ropeStart);

	for(int i = 0; i < rope::pieces; i++){
		ropePieces.push_back(nf.createRopePiece());
	}

	for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
		lightedScene.addChild((* it).getRootNode());
	}
	

	// TODO init plattforms
	
	// TODO init icicles
	
}

void GameModel::createRopePiece(){
	 
}