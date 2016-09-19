#pragma once
#include <GameModel.h>
#include <PhysicsController.h>
#include <time.h>

#include <OSGCSM/OSGCAVESceneManager.h>

class GameController
{
public:
	GameController(void);
	~GameController(void);

	void init(OSGCSM::CAVESceneManager* newMgr);

	void callGameLoop();

	GameModel * getModel();

	NodeTransitPtr setupScenegraph();
	bool isGrounded();

	int getGameState();
	void setGameState(int newGS);

	void moveHook(Vec3f direction, float speed);
	void resetGame();

	void resetHook();
	void resetRope();
	void startGame();

	void jumpToNextPlattform();
	void jumpToPreviousPlattform();

private:
	GameModel * model;
	PhysicsController * pCtrl;
	OSGCSM::CAVESceneManager *mgr;

	bool hookFlying;

	Vec3f tossVector;

	int currentTick;
	int readyToChangeState;
	int gameState;
	bool prepToStop;
	int currentPltForm;

	clock_t startTime;

	int calcNewTick();
	void changeCurrentState(int newState);
	void jumpToPltForm(int i);

	void moveTowardsPlattform();

	int count;
	int currentTickCount;

	int countBounce;
};

