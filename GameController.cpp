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

	std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
	for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
		(* it).setDirection(Vec3f(0,0,0));
		// TODO
		(* it).setPosition(mgr->getTranslation() + Vec3f(0,0,0));
		(* it).getTransformation()->setScale(rope::scaleVector);
	}
}

void GameController::resetHook(){
	ropeOrigin = pltPositions::ropeOrigins[currentPltForm] * general::scale;
	model->getHook().setPosition(mgr->getTranslation() + Vec3f(0,0,0));
	model->getHook().setDirection(Vec3f(0,0,0));
	float rotation = mgr->getYRotate();
	Vec3f newLookAt = Vec3f(0,0,-1);
	newLookAt = Matrix(
					Vec3f(cos(rotation), 	0, 	-sin(rotation)),
					Vec3f(0, 		1, 	0),
					Vec3f(sin(rotation),	0,	cos(rotation))
				) * newLookAt;
	model->getHook().setLookAt(newLookAt);

	resetRope();
}

void GameController::startGame(){
	resetHook();
	changeCurrentState(1);
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
	Vec3f hookMgrVec = model->getHook().getPosition() - mgr->getTranslation();
	float length = initialDirection.length();
	hookMgrVec.normalize();
	Vec3f tempVec = initialDirection;
	tempVec.normalize();
	Vec3f intialPortion = tempVec * 30;
	// Vec3f newDirection = Vec3f(hookMgrVec[0], 0, hookMgrVec[2]) + (initialDirection[1] < 0 ? Vec3f(0,0,0) : rope::gravityVector) + intialPortion;
	Vec3f newDirection = Vec3f(hookMgrVec[0], 0, hookMgrVec[2]) + rope::gravityVector + intialPortion;
	newDirection.normalize();
	//newDirection = Vec3f(newDirection[0], newDirection[1] < 0 ? 0 : newDirection[1], newDirection[2]);
	return newDirection * length;
}

void GameController::calculateRopeDirection(){
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
		int dt = 1;
		if(ctrlPnt1.length() != 0){
			// initialDirection.normalize();
			p0 = ctrlPnt3;
			p1 = ctrlPnt1;
			dt = 2;
			// lastObjPosition = ctrlPnt1; //
		}

		for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
			if(count == ropeListSize / 2 && ctrlPnt1.length() != 0){
				count = 0;
				p0 = ropeOrigin;
				p1 = ropeOrigin + initialDirection;
				p2 = ctrlPnt1;
				p3 = ctrlPnt2;
				// p2 = hook.getPosition() - hook.getLookAt();
				// p3 = hook.getPosition();
				// lastObjPosition = hook.getPosition(); //
			}
			count++;
			Vec3f currentPosition = (* it).getPosition();
			Vec3f currentDirection = (* it).getDirection();
			float t = 1.0f - float(count) / float(ropeListSize / dt);
			Vec3f bezierPoint0 = lastObjPosition;
			Vec3f bezierPoint1 = getBezierPoint(p0, p1, p2, p3, t);
			Vec3f bezierDirection = (bezierPoint0 - bezierPoint1) / 2;

			float scale = bezierDirection.length();
			Vec3f newScale = Vec3f(rope::scaleVector[0] * general::scale, rope::scaleVector[1] * general::scale, scale);
			(* it).setDirection(bezierDirection);
			if(count == ropeListSize && ropeLength > general::scale * rope::maxRopeLength && false){
				// std::cout << "ropeLength: " << ropeLength << "\n";
				(* it).setPosition(lastObjPosition);
				(* it).getTransformation()->setScale(newScale);
				MatrixLookAt((* it).getPosition(), lastObj.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				// ropeOrigin = (* it).getPosition();
			} else {
				Vec3f newPosition = bezierPoint1 + bezierDirection;
				(* it).setPosition(newPosition);
				(* it).getTransformation()->setScale(newScale);
				MatrixLookAt((* it).getPosition(), lastObjPosition, Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				if(hook.getDirection().length() > 0)
					ropeLength += scale;
			}
			
			// TODO: Länge des Seils berechnen
			
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

void GameController::callGameLoop(){
	int newTick = calcNewTick();
	if(newTick != currentTick){
		currentTickCount++;
		VRGPhysicsObject hook = model->getHook();
		currentTick = newTick;
		VRGPhysicsObject ropeStart = model->getRopeStart();
		if(gameState == 1){ 
			Vec3f hookDirection = hook.getDirection();
			if(hookDirection.length() > 0){
				Vec3f distanceVector = hook.getPosition() - mgr->getTranslation();
				Line l = Line(mgr->getTranslation(), distanceVector);
				Vec4f overthrow = pCtrl->overthrow(l, model->getCave(), distanceVector.length());
				if(overthrow[3] != -1 && ctrlPnt1.length() == 0){
					
					Vec3f hookDirection = hook.getDirection();
					hookDirection.normalize();
					ctrlPnt1 = Vec3f(overthrow[0], overthrow[1], overthrow[2]);
					std::cout << "hit point" << ctrlPnt1 << "\n";
					ctrlPnt2 = ctrlPnt1 + hookDirection;
					ctrlPnt3 = ctrlPnt1 - hookDirection;
				}

				calculateRopeDirection();
				calculateRopeLookAt();
				pCtrl->calculateNewTickForPhysicsObject(hook);
				if(hook.getDirection().length() > 0){
					bool didHit = pCtrl->collision(hook, model->getCave());
					if(didHit){
						// TODO
						int pltformHit = pCtrl->didHitPLattform(hook);
						if(pltformHit != -1 && currentPltForm != pltformHit && ropeLength < general::scale * rope::maxRopeLength){ // Plattform getroffen => Seil spannen
							std::cout << "plattform hit" << std::endl;
							currentPltForm = pltformHit;
							hook.setDirection(Vec3f(0,0,0));
							Vec3f cavePosition = mgr->getTranslation() - Vec3f(0,1,0);
							Vec3f lookDirection =  hook.getPosition() - cavePosition;
							MatrixLookAt(cavePosition, hook.getPosition() + lookDirection, Vec3f(0,1,0), hook.getTransformation()->editRotation());
							changeCurrentState(2);
						} else {
							// es wurde nur die Cave getroffen, keine Plattform
							countBounce++;
						}
					}
				}
			}
			if(!isGrounded()){
				mgr->setTranslation(mgr->getTranslation() - general::upVector * general::scale);
			}
		} else if (gameState == 2){ 
			Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale;
			Vec3f direction = pltformPosition - hook.getPosition();
			int countRopePieces = currentTickCount;
			Vec3f hookPosition = hook.getPosition();
			Vec3f mgrPosition = mgr->getTranslation() - Vec3f(0,1,0);
			Vec3f mgrHook = hookPosition - mgrPosition;
			std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
			VRGPhysicsObject lastObj = hook;
			Vec3f lastObjPosition = hook.getPosition();
			int count = 0;
			int inLine = 0;
			if(direction.length() > physics::minDirectionLengthValue * general::scale){
				hook.setPosition(hook.getPosition() + (direction * 0.02));
			} else {
				hook.setPosition(pltformPosition);
				MatrixLookAt(hook.getPosition(), hook.getPosition() + mgrHook, Vec3f(0,1,0), hook.getTransformation()->editRotation());
			}
			for (std::list<VRGPhysicsObject>::iterator it=ropePieces.begin(); it != ropePieces.end(); ++it){
				count++;
				Vec3f targetPosition = mgrPosition + (mgrHook / ropePieces.size()) * (ropePieces.size() - count);
				Vec3f currentPosition = (* it).getPosition();
				Vec3f targetVector = targetPosition - currentPosition;
				if(targetVector.length() > 0.05){
					//targetVector.normalize();
					Vec3f positionVector = lastObj.getPosition() - currentPosition;
					Vec3f newScale = Vec3f(rope::scaleVector[0] * general::scale,rope::scaleVector[1] * general::scale,positionVector.length());
					(* it).getTransformation()->setScale(newScale * 0.75);
					(* it).setPosition(currentPosition + targetVector * 0.06);
					MatrixLookAt((* it).getPosition(), lastObj.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				} else {
					inLine++;
					// std::cout << "inline: " << inLine << "\n";
					MatrixLookAt((* it).getPosition(), hook.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				}

				lastObj = (* it);
				lastObjPosition = currentPosition;

				//Vec3f rpPosition = (* it).getPosition();
				//Vec3f mgrRp = rpPosition - mgrPosition;
				//Vec3f rpHook = hookPosition - rpPosition;
				//rpHook.normalize();
				//mgrRp.normalize();
				//mgrHook.normalize();
				//
				//Vec3f mgrUp = mgrHook.cross(mgrRp);
				//mgrUp.normalize();
				//Vec3f mgrRight = mgrUp.cross(mgrHook) ;
				//mgrRight.normalize();
				//mgrRight = mgrRight + mgrRp;
				////Vec3f lookAtRopePiece = lastObjPosition - rpPosition;
				////lookAtRopePiece.normalize();
				//float dotProd = mgrHook * rpHook;
				//if(dotProd < 0.01){
				//	std::cout << "dotProd: " << dotProd << "\n";
				//	count++;
				//	MatrixLookAt((* it).getPosition(), hook.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				//}
				//else {
				//	Vec3f positionVector = lastObj.getPosition() - rpPosition;
				//	Vec3f newScale = Vec3f(rope::scaleVector[0] * general::scale,rope::scaleVector[1] * general::scale,positionVector.length());
				//	(* it).getTransformation()->setScale(newScale * 0.75);
				//	(* it).setPosition(rpPosition - mgrRight * 3);
				//	MatrixLookAt((* it).getPosition(), lastObj.getPosition(), Vec3f(0,1,0), (* it).getTransformation()->editRotation());
				//}

				/*lastObj = (* it);
				lastObjPosition = currentPosition;*/
			}

			if(direction.length() < physics::minDirectionLengthValue * general::scale && inLine >= ropePieces.size()){
				changeCurrentState(3);
			}
			// TODO: changeState(3) Bedingung �ndern, Seil muss auch entsprechend animiert werden

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
	Vec3f pltformPosition = pltPositions::positions[currentPltForm] * general::scale;
	Vec3f direction = pltformPosition - mgr->getTranslation();
	float dLength = direction.length();
	float dScale = 30;
	dScale = (dScale > dLength ? dLength : dScale);
	direction.normalize();
	// if(direction.length() > physics::minDirectionLengthValue * general::scale){
	if(mgr->getTranslation() != pltformPosition){
		mgr->setTranslation(mgr->getTranslation() + direction * dScale);
	} else {
		ropeOrigin = pltPositions::positions[currentPltForm];
		// resetHook();

		mgr->setTranslation(pltformPosition);
		changeCurrentState(1);
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
	initialDirection = Vec3f(0,0,0);

	ctrlPnt1 = Vec3f(0,0,0);
	ctrlPnt2 = Vec3f(0,0,0);
	ctrlPnt3 = Vec3f(0,0,0);

	ropeLength = 0;
	ropeOrigin = pltPositions::positions[currentPltForm] * general::scale;
	linuxPrevTime = std::chrono::steady_clock::now();
}