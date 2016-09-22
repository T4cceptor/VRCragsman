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

	void setRopeOrigin(Vec3f newOrigin);

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

	Vec3f ropeOrigin;
	Vec3f initialDirection;

	Vec3f getBezierPoint(Vec3f a, Vec3f b, Vec3f c, Vec3f d, float t);

	void calculateRopeDirection();
	Vec3f calculateNewRopeDirection();

	void calculateRopeLookAt();
};

