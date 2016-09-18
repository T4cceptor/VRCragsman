#pragma once
#include "NodeFactory.h"
#include "PlattformObject.h"
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
	VRGPhysicsObject getCave();
	VRGPhysicsObject getPlayer();
	VRGPhysicsObject getHook();
	VRGPhysicsObject getRopeStart();
	std::list<VRGPhysicsObject> getRopeTail();
	std::list<PlattformObject> getPlattforms();
	std::list<VRGPhysicsObject> getIcicles();
	// PlattformObject getPlattformWithId();
	// VRGPhysicsObject getRopePieceWithId();
	// VRGPhysicsObject getIcicleWithId();
	VRGPhysicsObject createNewHook(Pnt3f pos, Vec3f direction);
	void moveHook(Pnt3f pos, Vec3f direction);

	//Init
	void initGameModel(PhysicsController * pc);
	void createScenegraph();

	// Physics
	PhysicsController physicCtrl;
	void createRopePiece();


private:
	// Init
	// TODO
	int gameState;

	// Game Objects
	NodeFactory nf;
	VRGPhysicsObject root;
	VRGPhysicsObject cave;
	VRGPhysicsObject player;
	VRGPhysicsObject hook;

	NodeRecPtr lightTopPtr;
	std::list<NodeRecPtr> staticCaveLights;
	VRGObject lightedScene;

	VRGPhysicsObject ropeStart;
	std::list<VRGPhysicsObject> ropePieces; // evtl. durch eigene Klasse beschreiben
	std::list<VRGPhysicsObject> icicles; // evtl. durch eigene Klasse beschreiben
	PlattformObject currentPlattform;
	std::list<PlattformObject> plattforms;
	VRGPhysicsObject allPlattforms;

};

