#include "GameController.h"
#include "Config.h"

extern float timeDelta;

void GameController::resetGame(){
	// TODO
	std::cout << "Game reset" << std::endl;
}

void GameController::changeCurrentState(int newState){
	//if(readyToChangeState == 0){
	//	readyToChangeState = newState;
	//	std::cout << "next state available: " << readyToChangeState << std::endl;
	//}
	gameState = newState;
	std::cout << "next state: " << gameState << std::endl;
}

void GameController::moveHook(Vec3f direction, float speed){
	// TODO: checken ob der Haken bereits fliegt!
	prepToStop = false;
	readyToChangeState = 0;
	model->moveHook(
		mgr->getTranslation() + direction * hook::movementOffsetScale * general::scale, 
		direction * hook::movementVectorScale * general::scale * speed
	);
	std::cout << "moving hook with speed: " << speed << std::endl;
}

void GameController::callGameLoop(){
		int newTick = calcNewTick();
		if(newTick != currentTick){
			VRGPhysicsObject hook = model->getHook();
			currentTick = newTick;
			if(gameState == 1){ 
				// TODO: animateRope();

				Vec3f direction = hook.getDirection();
				if(direction.length() > 0){
					Vec3f distanceVector = hook.getPosition() - mgr->getTranslation();
					if(distanceVector.length() > hook::maxDistanceValue * general::scale){
						// TODO
						std::cout << "max distance reached" << "\n";
						//float speed = hook.getDirection().length();
						//hook.setDirection(0,-3 * physics::gravity * general::scale,0);
						//changeCurrentState(4);
					}

					pCtrl->calculateNewTickForPhysicsObject(hook);
					if(hook.getDirection().length() > 0){
						bool didHit = pCtrl->collision(hook, model->getCave());
						// TODO: falls sich Position des Hakens über mehrere Ticks nicht ändert => direction = 0,0,0
						if(didHit){
							int pltformHit = pCtrl->didHitPLattform(hook);
							if(pltformHit != -1){
								std::cout << "plattform hit" << std::endl;
								currentPltForm = pltformHit;
								hook.setDirection(Vec3f(0,0,0));
								changeCurrentState(2);
							}
						}
					}
				}
				if(!isGrounded()){
					mgr->setTranslation(mgr->getTranslation() - general::upVector * general::scale);
				}
			} else if (gameState == 2){ // TODO: moveRope() !!!
				Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale;
				Vec3f direction = pltformPosition - hook.getPosition();
				if(direction.length() > physics::minDirectionLengthValue * general::scale){
					Vec3f newPosition = hook.getPosition() + (direction * 0.02);
					hook.setPosition(newPosition[0], newPosition[1], newPosition[2]);
				} else {
					hook.setPosition(pltformPosition[0], pltformPosition[1], pltformPosition[2]);
					changeCurrentState(3);
				}
			} else if (gameState == 3){ // TODO
				moveTowardsPlattform();
			} 

			// TODO: diese beiden GameStates evtl. rauslassen -> lieber Knopf um Haken + Seil zurück zu setzen
			else if (gameState == 4){ // TODO
				Vec3f distanceVector = hook.getPosition() - mgr->getTranslation();
				distanceVector.normalize();
				float dotProd = abs(distanceVector * Vec3f(0,1,0));
				// std::cout << "distanceVector: " << distanceVector << " ,dotProd: " << dotProd << "\n";
				if(dotProd < 0.98) // TODO
				{
					Vec3f newDirection = hook.getDirection() - distanceVector;
					Vec3f newPosition = hook.getPosition() + newDirection;
					hook.setPosition(newPosition[0], newPosition[1], newPosition[2]);
				} else {
					changeCurrentState(5);
				}
			} else if (gameState == 5){

				// TODO: Hook kommt zum Spieler zurück
				// Problem: Plattform ist im Weg ...
				// -> korrekte Bewegung nachahmen

				Vec3f targetPosition = mgr->getTranslation();
				Vec3f direction = targetPosition - hook.getPosition();
				if(direction.length() > physics::minDirectionLengthValue * general::scale){
					
					Vec3f newPosition = hook.getPosition() + (direction * 0.02);
					hook.setPosition(newPosition[0], newPosition[1], newPosition[2]);
					std::cout << "newPosition: " << newPosition << "\n";
				} else {
					hook.setDirection(Vec3f(0,0,0));
					hook.setPosition(targetPosition[0], targetPosition[1], targetPosition[2]);
					changeCurrentState(1);
				} 
			}
		}
}

void GameController::moveTowardsPlattform(){
	Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale;
	Vec3f direction = pltformPosition - mgr->getTranslation();
	if(direction.length() > physics::minDirectionLengthValue * general::scale){
		mgr->setTranslation(mgr->getTranslation() + direction * 0.02);
	} else {
		mgr->setTranslation(pltformPosition);
		changeCurrentState(1);
	}
}

int GameController::calcNewTick(){
	clock_t now = clock();
	clock_t delta = now - startTime;
	// timeDelta = static_cast<float>(delta);
	// int seconds_elapsed = static_cast<int>(delta) / CLOCKS_PER_SEC;
	int newTick = long(static_cast<long>(delta) / ( CLOCKS_PER_SEC / general::ticksPerSecond)) % general::ticksPerSecond;
	return newTick;
}

bool GameController::isGrounded(){
	Line ray = Line(mgr->getTranslation() - Vec3f(0, 1 * general::scale, 0), Vec3f(0,-1,0));
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	iAct->setLine(ray);
	NodeRefPtr someNode = model->getCave().getRootNode();
	iAct->apply((Node * const)someNode);
	if (iAct->didHit())
	{
		float dis = (iAct->getHitPoint().subZero() - mgr->getTranslation()).length();
		if(dis > general::minDistanceToFloor * general::scale ){
			return false;
		}
		return true;
	}
	return false;
}

int GameController::getGameState(){
	return gameState;
}

void GameController::setGameState(int newGS){
	gameState = newGS;
}

GameModel * GameController::getModel(){
	return model;
}

NodeTransitPtr GameController::setupScenegraph(){
	model->createScenegraph();
	std::cout << "setting up scenegraph" << std::endl;
	return model->getScenegraphRoot().getRootNode();
}

GameController::GameController(void)
{
	std::cout << "GameController contructor called" << std::endl;
}

GameController::~GameController(void)
{
}

void GameController::init(OSGCSM::CAVESceneManager* newMgr){
	mgr = newMgr;
	pCtrl = new PhysicsController();
	model = new GameModel();
	model->initGameModel(pCtrl);

	currentTick = 0;
	readyToChangeState = 0;
	gameState = 1;
	startTime = clock();
	prepToStop = false;
	currentPltForm = 0;
}