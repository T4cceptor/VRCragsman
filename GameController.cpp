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

	//if(hookFlying)
	//	return;
	// TODO: checken ob der Haken bereits fliegt!
	countBounce = 0;
	count = 0;
	currentTickCount = 0;
	tossVector = direction * hook::movementVectorScale * general::scale * speed;
	prepToStop = false;
	readyToChangeState = 0;
	ropeLength = 0;
	initialDirection = direction * hook::movementVectorScale * general::scale * speed;
	model->moveHook(
		ropeOrigin, 
		initialDirection
		);
}

void GameController::resetRope() {
	ropeLength = 0;
	ctrlPnt1 = Vec3f(0,0,0);

	calculateRopeDirection();

	//std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
	//for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
	//	(* it).setDirection(Vec3f(0,0,0));
	//	// TODO
	//	(* it).setPosition(mgr->getTranslation() + Vec3f(0,0,0));
	//	(* it).getTransformation()->setScale(rope::scaleVector);
	//}
}

void GameController::resetHook(){
	ropeHitCave = -1;
	initialDirectionScale = 30;
	Vec3f newHookPosition = ropeOrigin + Vec3f(100,0,-50);
	model->getHook().setPosition(newHookPosition);
	model->getHook().setDirection(Vec3f(0,0,0));
	Vec3f newLookAt = Vec3f(1,0,-1);
	float rotation = mgr->getYRotate();
	newLookAt = Matrix(
					Vec3f(cos(rotation), 	0, 	-sin(rotation)),
					Vec3f(0, 		1, 	0),
					Vec3f(sin(rotation),	0,	cos(rotation))
				) * newLookAt;
	model->getHook().setLookAt(newLookAt);
	MatrixLookAt(newHookPosition, newLookAt, Vec3f(0,1,0), model->getHook().getTransformation()->editRotation());
}


void GameController::startGame(){
	changeCurrentState(1);
	resetGameState(1);
}

void GameController::resetAnchor(){
	model->getAnchor().setPosition(ropeOrigin);
}

void GameController::resetGameState(int i){
	ropeOrigin = pltPositions::ropeOrigins[currentPltForm] * general::scale;
	shortestDistanceToCave = 10000;
	resetHook();
	resetAnchor();
	resetRope();
}

void GameController::jumpToNextPlattform(){
	int i = currentPltForm + 1;
	currentPltForm = (i >= pltPositions::size ? 0 : i);
	jumpToPltForm(currentPltForm);
}

void GameController::jumpToPreviousPlattform(){
	int i = currentPltForm - 1;
	currentPltForm = (i < 0 ? pltPositions::size - 1 : i);
	jumpToPltForm(currentPltForm);
}

void GameController::jumpToPltForm(int i){
	mgr->setTranslation(pltPositions::positions[currentPltForm] * general::scale);
	mgr->setYRotate(pltPositions::rotation[currentPltForm]);
	std::cout << "moving to position: " << currentPltForm << '\n';
}

void GameController::setRopeOrigin(Vec3f newOrigin){
	ropeOrigin = newOrigin;
}

Vec3f GameController::getBezierPoint(Vec3f origin, Vec3f p1, Vec3f p2, Vec3f target, float t){
	float u = 1 - t;
	float tt = t*t;
	float uu = u*u;
	float uuu = uu * u;
	float ttt = tt * t;
	Vec3f bp = uuu * origin;
	bp += 3 * uu * t * p1;
	bp += 3 * u * tt * p2;
	bp += ttt * target;
	return bp;
}

Vec3f GameController::calculateNewRopeDirection(){
	

	Vec3f from = ropeOrigin;
	Vec3f at = (ctrlPnt1.length() > 0 && ropeHitCave > 0) ? ctrlPnt1 : model->getHook().getPosition();
	Vec3f direction = at - from;
	if(gameState == 2){
		return direction;
	}

	float length = initialDirection.length();
	float min = 1000;
	float max = 1000;
	length = direction.length() / 8;
	length = length < max ? length : max; // max Wert einstellen - 100

	direction.normalize();
	Vec3f tempVec = initialDirection;
	tempVec.normalize();
	Vec3f intialPortion = tempVec * initialDirectionScale;
	Vec3f newDirection = Vec3f(direction[0], 0, direction[2]) + rope::gravityVector + intialPortion;
	newDirection.normalize();

	newDirection = Vec3f(newDirection[0], newDirection[1] < ropeOrigin[1] ? ropeOrigin[1] : newDirection[1], newDirection[2]);
	return newDirection * length;
}

float distanceToLine(Vec3f lineStart, Vec3f lineEnd, Vec3f point) {
	Vec3f x0 = lineStart;
	Vec3f x1 = lineEnd;
	Vec3f x2 = point;
	//return ((x0 - x1).cross(x0 - x2)).length() / ((x2 - x1).length());
	return ((x0 - x2).cross(x0 - x1)).length() / ((x0 - x1).length());
} 

int pointRelationToLine(Vec3f s, Vec3f e, Vec3f p){
	// p liegt auf y-Achse zwischen s und e UND (entweder auf x-Achse zwischen s und e ODER auf z-Achse zwischen s und e)
	if((s[1] >= p[1] && e[1] <= p[1]) && 
		((s[0] >= p[0] && e[0] <= p[0]) || (s[0] < p[0] && e[0] > p[0])) ||
		((s[2] >= p[2] && e[2] <= p[2]) || (s[2] < p[2] && e[2] > p[2]))
		){
		Vec3f v1 = e - s;
		v1.normalize();
		Vec3f v2 = e - p;
		v2.normalize();
		// falls v2.y > v1.y => p liegt über der Strecke s->e, falls 0 => p liegt auf s->e, andernfalls darunter
		if(v2[1] > v1[1]){
			return 1;
		} else if(v2[1] < v1[1]){
			return -1;
		}
		else
			return 0;
	}
	return 0;
}

void GameController::calculateRopeDirection(){
	float shortestDistance = 1000;
	VRGPhysicsObject hook = model->getHook();
	initialDirection = calculateNewRopeDirection();
	std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
	int ropeListSize = ropePieces.size();
	if(ropeListSize > 0){
		int count = 0;
		VRGPhysicsObject lastObj = hook;
		Vec3f lastObjPosition = hook.getPosition();
		
		Vec3f p0 = ropeOrigin;
		Vec3f p1 = ropeOrigin + initialDirection;
		Vec3f p2 = hook.getPosition() - hook.getLookAt();
		Vec3f p3 = hook.getPosition();
		
		if(ctrlPnt1.length() != 0 && ropeHitCave > 0){
			p0 = ctrlPnt1;
			p1 = ctrlPnt3;
		}
		int amountOfRopePieces = ropeListSize - (ropeHitCave > 0 ? ropeHitCave : 0);
		for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){			
			count++;
			Vec3f currentPosition = (* it).getPosition();
			Vec3f currentDirection = (* it).getDirection();

			if(count == amountOfRopePieces && ropeHitCave > 0){ 
				p0 = ropeOrigin;
				p1 = ropeOrigin + initialDirection;
				p2 = ctrlPnt2;
				p3 = ctrlPnt1;
				count = 1;
				lastObjPosition = ctrlPnt1;
				amountOfRopePieces = ropeHitCave;
			}
			
			if(ctrlPnt1.length() != 0 && ropeHitCave == -1){
				Vec3f lineStart = currentPosition - currentDirection * 2;
				Vec3f lineEnd = currentPosition + currentDirection * 2;
				if(pointRelationToLine(lineStart, lineEnd, ctrlPnt1) == 1){
					// std::cout << "count: " << count << "\n";
					Vec3f distanceVector = lineEnd - lineStart;
						Line l = Line(currentPosition, distanceVector);
						Vec4f overthrow = pCtrl->overthrow(l, model->getCave(), distanceVector.length());
						if(overthrow[3] != -1){ // if overthrow[3] != -1 => overthrow[0-2] = hitPoint
							ctrlPnt1 = Vec3f(overthrow[0], overthrow[1], overthrow[2]);
							// std::cout << "hit point" << ctrlPnt1 << "\n";
							Vec3f hookDirection = model->getHook().getDirection();
							hookDirection.normalize();
							ctrlPnt2 = ctrlPnt1 + hookDirection;
							ctrlPnt3 = ctrlPnt1 - hookDirection;

							ropeHitCave = ropeListSize / 4;
							initialDirection.normalize();
						}
				}
			}
			
			float t = 1.0f - float(count) / float(amountOfRopePieces);
			Vec3f bezierPoint0 = lastObjPosition;
			Vec3f bezierPoint1 = getBezierPoint(p0, p1, p2, p3, t);
			Vec3f bezierDirection = (bezierPoint0 - bezierPoint1) / 2;
			float scale = bezierDirection.length();
			Vec3f newScale = Vec3f(rope::scaleVector[0] * general::scale, rope::scaleVector[1] * general::scale, scale);
			// std::cout << "scale: " << scale << "\n";
			(* it).setDirection(bezierDirection);
			Vec3f newPosition = bezierPoint1 + bezierDirection;
			if(p3 == ctrlPnt1){
				(* it).setPosition(newPosition);
				(* it).getTransformation()->setScale(newScale);
				Vec3f lookAt = Vec3f(lastObjPosition[0], lastObjPosition[1] < ctrlPnt1[1] ? ctrlPnt1[1] : lastObjPosition[1], lastObjPosition[2]);
				MatrixLookAt((* it).getPosition(), lookAt, Vec3f(0,1,0), (* it).getTransformation()->editRotation());
			} else {
				(* it).setPosition(newPosition);
				(* it).getTransformation()->setScale(newScale);
				MatrixLookAt((* it).getPosition(), lastObjPosition, Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				if(hook.getDirection().length() > 0)
					ropeLength += scale;
			}
			lastObj = (* it);
			lastObjPosition = bezierPoint1;
		}
	}
}

void GameController::calculateRopeLookAt(){
	VRGPhysicsObject hook = model->getHook();
	Vec3f lookAt = hook.getLookAt();
	Vec3f direction = hook.getDirection();
	float length = lookAt.length();
	lookAt.normalize();
	direction.normalize();
	float dotProd = lookAt * direction;
	Vec3f newLookAt = lookAt + direction * 0.1;
	newLookAt.normalize();
	hook.setLookAt(newLookAt);
}

void GameController::resetBezier(){
	ctrlPnt1 = Vec3f(0,0,0);
	ropeHitCave = -1;
}

void GameController::callGameLoop(){
	int newTick = calcNewTick();
	if(newTick != currentTick){
		currentTick = newTick;
		currentTickCount++;
		VRGPhysicsObject ropeStart = model->getRopeStart();
		VRGPhysicsObject hook = model->getHook();
		Vec3f hookPosition = hook.getPosition();
		Vec3f hookDirection = hook.getDirection();
		float hookDirectionLength = hookDirection.length();
		// MatrixLookAt(ropeOrigin, Vec3f(hookPosition[0], 0, hookPosition[2]), Vec3f(0,1,0), model->getAnchor().getTransformation()->editRotation());						
		if(gameState == 1){ 
			if(hookDirectionLength > 0){
				pCtrl->calculateNewTickForPhysicsObject(hook);
				if(hookDirectionLength > 0){
					bool didHit = pCtrl->collision(hook, model->getCave());
					if(didHit){
						// TODO
						// std::cout << "cave hit" << std::endl;
						int pltformHit = pCtrl->didHitPLattform(hook);
						if(pltformHit != -1 && currentPltForm != pltformHit){ // Plattform getroffen => Seil spannen
							// std::cout << "plattform hit" << std::endl;
							currentPltForm = pltformHit;
							hook.setDirection(Vec3f(0,0,0));
							ctrlPnt1 = Vec3f(0,0,0);
							Vec3f cavePosition = mgr->getTranslation() - Vec3f(0,1,0);
							Vec3f lookDirection =  hook.getPosition() - cavePosition;
							MatrixLookAt(cavePosition, hook.getPosition() + lookDirection, Vec3f(0,1,0), hook.getTransformation()->editRotation());
							changeCurrentState(2);
						} else {
							// es wurde nur die Cave getroffen, keine Plattform
							countBounce++;
						}
					}

					if(gameState == 1 && hook.getDirection().length() > 0){ // && !didHit
						if(!didHit){
						// test if cave part was overthrown by the hook
						Vec3f distanceVector = (hookPosition) - ropeOrigin;
						Line l = Line(ropeOrigin, distanceVector);
						Vec4f overthrow = pCtrl->overthrow(l, model->getCave(), distanceVector.length());
						hookDirection.normalize();
						if(overthrow[3] != -1 && ctrlPnt1.length() == 0){ // if overthrow[3] != -1 => overthrow[0-2] = hitPoint
							if(ropeHitCave == -1)
								ropeHitCave = model->getRopeTail().size() / 4;
							ctrlPnt1 = Vec3f(overthrow[0], overthrow[1], overthrow[2]);
							ctrlPnt2 = ctrlPnt1 + hookDirection;
							ctrlPnt3 = ctrlPnt1 - hookDirection;
							std::cout << "hit point" << ctrlPnt1 << "\n";
						}  else if(overthrow[3] == -1 && (ctrlPnt1.length() != 0 || ropeHitCave > 0)){
							resetBezier();
						} 
							calculateRopeDirection();
							calculateRopeLookAt();
						}
					}
				}
			}
			if(!isGrounded()){
				mgr->setTranslation(mgr->getTranslation() - general::upVector * general::scale);
			}
		} else if (gameState == 2){
			resetBezier();
			Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale;
			Vec3f hookPosition = hook.getPosition();
			Vec3f direction = pltformPosition - hookPosition;
			Vec3f mgrHook = hookPosition - ropeOrigin;

			std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
			VRGPhysicsObject lastObj = hook;
			int count = 0;
			int inLine = 0;
			if(direction.length() > physics::minDirectionLengthValue * general::scale){
				Vec3f tempDirection = direction;
				tempDirection.normalize();
				hook.setPosition(hookPosition + tempDirection * 10);
			} else {
				hook.setPosition(pltformPosition);
				hook.setLookAt(mgrHook);
				MatrixLookAt(hookPosition, hookPosition + mgrHook, Vec3f(0,1,0), hook.getTransformation()->editRotation());
			}

			if(direction.length() < physics::minDirectionLengthValue * general::scale){
				calculateRopeDirection();
				changeCurrentState(3);
			}

			calculateRopeDirection();
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
	if(gameState != 3){
		return;
	}
	Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale + pltPositions::offset; //+ pltPositions::offset;
	Vec3f direction = pltformPosition - mgr->getTranslation();
	float dLength = direction.length();
	float dScale = 30;
	dScale = (dScale > dLength ? dLength : dScale);
	direction.normalize();
	// if(direction.length() > physics::minDirectionLengthValue * general::scale){
	float circleRotation = 2 * general::pi;
	//float rotation = pltPositions::rotation[currentPltForm] < 0 ? 
	//	circleRotation + pltPositions::rotation[currentPltForm] : pltPositions::rotation[currentPltForm];
	if(mgr->getTranslation() != pltformPosition){
		mgr->setTranslation(mgr->getTranslation() + direction * dScale);
	} else {
		// 2 * pi 
		ropeOrigin = pltPositions::positions[currentPltForm];
		mgr->setTranslation(pltformPosition);
		// mgr->setYRotate(pltPositions::rotation[currentPltForm]);

		changeCurrentState(1);
		// resetGameState(1);
	}
}

int GameController::calcNewTick(){
	std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
	long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - linuxPrevTime).count();
	int newTick = elapsed / (1000 / general::ticksPerSecond);
	if(newTick == 0){
		newTick = currentTick;
	}
	else{
		linuxPrevTime = timeNow;
		newTick = (currentTick + 1) % general::ticksPerSecond;
	}
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
	elapsedTime = 0;
	hookFlying = false;
	shortestDistanceToCave = 10000;
	

	initialDirection = Vec3f(0,0,0);

	ctrlPnt1 = Vec3f(0,0,0);
	ctrlPnt2 = Vec3f(0,0,0);
	ctrlPnt3 = Vec3f(0,0,0);

	ropeLength = 0;
	ropeOrigin = pltPositions::ropeOrigins[currentPltForm] * general::scale;

	

	linuxPrevTime = std::chrono::steady_clock::now();
}