//#pragma once
#include "NodeFactory.h"
#include "PhysicsController.h"
#include <OSGNode.h>
#include <OSGSimpleGeometry.h>
#include "VRGLogger.h"
#include <OSGVector.h>
#include <sstream>
#include <thread>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class GameModel
{
public:
	GameModel(void);
	~GameModel(void);

	// Getters
	VRGObject getScenegraphRoot();
	VRGObject getCave();
	VRGObject getPltformRoot();
	VRGPhysicsObject getPlayer();
	VRGPhysicsObject getHook();
	VRGPhysicsObject getRopeStart();
	VRGPhysicsObject getAnchor();
	std::list<VRGPhysicsObject> getRopeTail();
	std::list<VRGPhysicsObject> getIcicles();
	VRGPhysicsObject createNewHook(Pnt3f pos, Vec3f direction);
	void moveHook(Pnt3f pos, Vec3f direction);

	//Init
	void initGameModel(PhysicsController * pc);
	void createScenegraph();

	// Physics
	PhysicsController physicCtrl;
	void createRopePiece();


private:
	int gameState;

	void setupLights();
	void setupIcicles();
	void setupAdditionalMeshes();
	void setupStalactites();
	void setupSpecialObjs();
	void setupPebbleGroup1();
	void setupPltforms();

	// Game Objects
	NodeFactory nf;
	VRGPhysicsObject root;
	VRGPhysicsObject cave;
	VRGPhysicsObject player;
	VRGPhysicsObject hook;
	VRGPhysicsObject anchor;
	VRGObject caveRoot;
	VRGObject platformRoot;
	VRGObject lightedScene;

	NodeRecPtr lightTopPtr;
	std::list<NodeRecPtr> staticCaveLights;
	
	VRGPhysicsObject ropeStart;
	std::list<VRGPhysicsObject> ropePieces; // evtl. durch eigene Klasse beschreiben
	std::list<VRGPhysicsObject> icicles; // evtl. durch eigene Klasse beschreiben
	VRGPhysicsObject allPlattforms;

};

