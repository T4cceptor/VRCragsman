#include "GameController.h"
#include "Config.h"

extern float timeDelta;
extern void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation);

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
	count = 0;
	tossVector = direction * hook::movementVectorScale * general::scale * speed;
	prepToStop = false;
	readyToChangeState = 0;
	model->moveHook(
		mgr->getTranslation() + direction * hook::movementOffsetScale * general::scale, 
		direction * hook::movementVectorScale * general::scale * speed
	);
	int c = 0;
	for (std::list<VRGPhysicsObject>::iterator it=model->getRopeTail().begin(); it != model->getRopeTail().end(); ++it){

		(* it).setDirection(direction * hook::movementVectorScale * general::scale * speed);
		(* it).setPosition(mgr->getTranslation());
		c++;
	}
	std::cout << "moving hook with speed: " << speed << std::endl;
}



void GameController::callGameLoop(){
		int newTick = calcNewTick();
		if(newTick != currentTick){
			VRGPhysicsObject hook = model->getHook();
			currentTick = newTick;
			VRGPhysicsObject ropeStart = model->getRopeStart();
			if(gameState == 1){ 
				Vec3f direction = hook.getDirection();
				if(direction.length() > 0){
					Vec3f distanceVector = hook.getPosition() - mgr->getTranslation();

					//if(tossVector.length() > 0){
					//	//int createTick = newTick % 2;
					//	//if(createTick == 0){
					//		if(hookDirection.length() > 0){
					//			ropeStart.setPosition(hook.getPosition() - hookDirection * 3 * hook::scale * general::scale);
					//			MatrixLookAt(ropeStart.getPosition(), hook.getPosition(), Vec3f(0,1,0), ropeStart.getTransformation()->editRotation());
					//		}
					//	//}
					//}
					// Vec3f hookDirection = hook.getDirection();

					// TODO: animateRope();

					//if(distanceVector.length() > hook::maxDistanceValue * general::scale){
					//	// TODO
					//	// std::cout << "max distance reached" << "\n";
					//	//float speed = hook.getDirection().length();
					//	//hook.setDirection(0,-3 * physics::gravity * general::scale,0);
					//	//changeCurrentState(4);
					//}

					Vec3f hookDirection = direction;
					if(hookDirection.length() > 0 ){
					std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
					if(ropePieces.size() > 0){
						int count = 0;
						VRGPhysicsObject lastObj = hook;
						Vec3f lastObjPosition = hook.getPosition();
							for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
								Vec3f tempPosition = (* it).getPosition();
								count++;
								//Vec3f toNextPieceVector = lastObj.getPosition() - (* it).getPosition();
								//toNextPieceVector.normalize();
								//float speed = hookDirection.length();
								//hookDirection.normalize();
								//Vec3f newDirection = (* it).getDirection() + toNextPieceVector;
								//newDirection.normalize();
								if((tempPosition - lastObjPosition).length() > 0.1){
								// (* it).setDirection(newDirection * speed);
									(* it).setDirection(lastObj.getDirection());
								// (* it).addTranslation((* it).getDirection());
									(* it).setPosition(lastObjPosition);
								
								//std::cout << "newDirection: " << newDirection * speed << "\n";
								//std::cout << "newTranslation: " << (* it).getPosition() << "\n";
									MatrixLookAt((* it).getPosition(), lastObj.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
									
									lastObj = (* it);
								} else
									break;
								lastObjPosition = tempPosition;
							}
						}
					}

					pCtrl->calculateNewTickForPhysicsObject(hook);
					if(hook.getDirection().length() > 0){
						bool didHit = pCtrl->collision(hook, model->getCave());
						// TODO: falls sich Position des Hakens �ber mehrere Ticks nicht �ndert => direction = 0,0,0
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
					//Vec3f newPosition = hook.getPosition() + (direction * 0.02);
					hook.setPosition(hook.getPosition() + (direction * 0.02));
				} else {
					hook.setPosition(pltformPosition);
					changeCurrentState(3);
				}
			} else if (gameState == 3){ // TODO
				moveTowardsPlattform();
			} 

			// TODO: diese beiden GameStates evtl. rauslassen -> lieber Knopf um Haken + Seil zur�ck zu setzen
			else if (gameState == 4){ // TODO
				Vec3f distanceVector = hook.getPosition() - mgr->getTranslation();
				distanceVector.normalize();
				float dotProd = abs(distanceVector * Vec3f(0,1,0));
				// std::cout << "distanceVector: " << distanceVector << " ,dotProd: " << dotProd << "\n";
				if(dotProd < 0.98) // TODO
				{
					Vec3f newDirection = hook.getDirection() - distanceVector;
					Vec3f newPosition = hook.getPosition() + newDirection;
					hook.setPosition(newPosition);
				} else {
					changeCurrentState(5);
				}
			} else if (gameState == 5){

				// TODO: Hook kommt zum Spieler zur�ck
				// Problem: Plattform ist im Weg ...
				// -> korrekte Bewegung nachahmen

				Vec3f targetPosition = mgr->getTranslation();
				Vec3f direction = targetPosition - hook.getPosition();
				if(direction.length() > physics::minDirectionLengthValue * general::scale){
					
					// Vec3f newPosition = hook.getPosition() + (direction * 0.02);
					hook.setPosition(hook.getPosition() + (direction * 0.02));
					// std::cout << "newPosition: " << newPosition << "\n";
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