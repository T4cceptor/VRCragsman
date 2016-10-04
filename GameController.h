// #pragma once
#include <time.h>
#include <chrono>
#include <OSGCSM/OSGCAVESceneManager.h>
#include "GameModel.h"

//using Clock = std::chrono::high_resolution_clock;
//using TimePoint = std::chrono::time_point<Clock>;

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
	void resetAnchor();
	void resetGameState(int i);
	void startGame();

	void jumpToNextPlattform();
	void jumpToPreviousPlattform();

	void setRopeOrigin(Vec3f newOrigin);
	void moveTowardsPlattform(float length);
	void moveTowardsPlattform(Vec3f movementDirection);

	bool rightMouseDown;
	bool leftMouseDown;

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
	std::chrono::steady_clock::time_point linuxPrevTime;
	long elapsedTime;
	
	int calcNewTick();
	void changeCurrentState(int newState);
	void jumpToPltForm(int i);

	int count;
	int currentTickCount;

	int countBounce;
	float ropeLength;

	Vec3f ropeOrigin;
	Vec3f initialDirection;
	int initialDirectionScale;

	Vec3f getBezierPoint(Vec3f a, Vec3f b, Vec3f c, Vec3f d, float t);

	void calculateRopeDirection();
	Vec3f calculateNewRopeDirection();

	void calculateRopeLookAt();

	Vec3f ctrlPnt1;
	Vec3f ctrlPnt2;
	Vec3f ctrlPnt3;

	int ropeHitCave;
	float shortestDistanceToCave;

	void resetBezier();
};

