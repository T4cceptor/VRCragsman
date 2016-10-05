#include "GameController.h"
#include "Config.h"

extern void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation);

GameController::GameController(void){ std::cout << "GameController contructor called" << std::endl; }
GameController::~GameController(void){}

void GameController::resetGame(){
	currentPltForm = 0;
	currentTick = 0;
	resetGameState(1);
	std::cout << "Game reset" << std::endl;
}

void GameController::changeCurrentState(int newState){
	gameState = newState;
	std::cout << "next state: " << gameState << std::endl;
}

void GameController::moveHook(Vec3f direction, float speed){
	count = 0;
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
	ctrlPnt2 = ctrlPnt1;
	ctrlPnt3 = ctrlPnt1;
	calculateRopePositions();
}

void GameController::resetHook(){
	ropeHitCave = -1;
	initialDirectionScale = 30;
	float rotation = mgr->getYRotate();
	Matrix rMatrix = Matrix(
		Vec3f(cos(rotation), 	0, 	-sin(rotation)),
		Vec3f(0, 		1, 	0),
		Vec3f(sin(rotation),	0,	cos(rotation))
		);
	Vec3f hookPositionOffset = rMatrix * Vec3f(100,-10,-50);
	Vec3f newHookPosition = ropeOrigin + hookPositionOffset;
	Vec3f newLookAt = Vec3f(1,0,-1);
	newLookAt.normalize();
	newLookAt = rMatrix * newLookAt;
	model->getHook().setPosition(newHookPosition);
	model->getHook().setDirection(Vec3f(0,0,0));
	model->getHook().setLookAt(newLookAt);
	MatrixLookAt(newHookPosition, newHookPosition + newLookAt, Vec3f(0,1,0), model->getHook().getTransformation()->editRotation());
}

void GameController::startGame(){
	changeCurrentState(1);
	resetGameState(1);
}

void GameController::resetAnchor(){
	model->getAnchor().setPosition(ropeOrigin);
}

void GameController::resetGameState(int i){
	gameState = i;
	setRopeOrigin(platforms::ropeOrigins[currentPltForm] * general::scale);
	resetHook();
	resetAnchor();
	resetRope();
}

void GameController::jumpToNextPlattform(){
	int i = currentPltForm + 1;
	currentPltForm = (i >= platforms::count ? 0 : i);
	jumpToPltForm(currentPltForm);
}

void GameController::jumpToPreviousPlattform(){
	int i = currentPltForm - 1;
	currentPltForm = (i < 0 ? platforms::count - 1 : i);
	jumpToPltForm(currentPltForm);
}

void GameController::jumpToPltForm(int i){
	mgr->setTranslation(platforms::positions[currentPltForm] * general::scale);
	setRopeOrigin(platforms::ropeOrigins[currentPltForm] * general::scale);
	resetGameState(i);
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
	if(gameState != 1){
		direction.normalize();
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
	newDirection = Vec3f(newDirection[0], newDirection[1] < ropeOrigin[1] ? ropeOrigin[1] : newDirection[1], newDirection[2]);
	newDirection.normalize();
	return newDirection * length * 0.9;
}

//checks if point p is below the line(s, e) -> returns -1, on the line -> returns 0, or above the line -> returns 1
int pointRelationToLine(Vec3f s, Vec3f e, Vec3f p){
	if((s[1] >= p[1] && e[1] <= p[1]) && 
		((s[0] >= p[0] && e[0] <= p[0]) || (s[0] < p[0] && e[0] > p[0])) ||
		((s[2] >= p[2] && e[2] <= p[2]) || (s[2] < p[2] && e[2] > p[2]))
		){
			Vec3f v1 = e - s;
			v1.normalize();
			Vec3f v2 = e - p;
			v2.normalize();
			if(v2[1] >= v1[1]){
				return 1;
			} else if(v2[1] < v1[1]){
				return -1;
			}
			else
				return 0;
	}
	return 0;
}

void GameController::calculateRopePositions(){
	std::list<VRGPhysicsObject> ropePieces = model->getRopeTail();
	int ropeListSize = ropePieces.size();
	if(ropeListSize > 0){
		initialDirection = calculateNewRopeDirection();
		VRGPhysicsObject hook = model->getHook();
		int count = 0;
		VRGPhysicsObject lastObj = hook;
		Vec3f lastObjPosition = hook.getPosition();
		bool useBezier = ctrlPnt1.length() != 0 && ropeHitCave > 0;
		Vec3f p0 = useBezier ? ctrlPnt1 : ropeOrigin;
		Vec3f p1 = useBezier ? ctrlPnt3 : ropeOrigin + initialDirection;
		Vec3f p2 = hook.getPosition() - hook.getLookAt();
		Vec3f p3 = hook.getPosition();
		int amountOfRopePieces = ropeListSize - (ropeHitCave > 0 ? ropeHitCave : 0);
		for (std::list<VRGPhysicsObject>::iterator it = ropePieces.begin(); it != ropePieces.end(); ++it){	
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

			float t = 1.0f - float(count) / float(amountOfRopePieces);
			Vec3f bezierPoint0 = lastObjPosition;
			Vec3f bezierPoint1 = getBezierPoint(p0, p1, p2, p3, t);
			Vec3f bezierDirection = (bezierPoint0 - bezierPoint1) / 2;
			float scale = bezierDirection.length();
			Vec3f newScale = Vec3f(rope::scaleVector[0] * general::scale, rope::scaleVector[1] * general::scale, scale);
			Vec3f newPosition = bezierPoint1 + bezierDirection;

			(* it).setDirection(bezierDirection);
			(* it).setPosition(newPosition);
			(* it).getTransformation()->setScale(newScale);
			Vec3f lookAt = lastObjPosition;

			if(p3 == ctrlPnt1){
				lookAt = Vec3f(lastObjPosition[0], lastObjPosition[1] < ctrlPnt1[1] ? ctrlPnt1[1] : lastObjPosition[1], lastObjPosition[2]);
			}
			MatrixLookAt((* it).getPosition(), lookAt, Vec3f(0,1,0), (* it).getTransformation()->editRotation());

			if(hook.getDirection().length() > 0){
				ropeLength += scale;
			}
			if(ctrlPnt1.length() != 0 && ropeHitCave == -1){
				Vec3f lineStart = currentPosition;
				Vec3f lineEnd = lastObjPosition;
				float startDistance = (ctrlPnt1 - lineStart).length();
				float endDistance = (ctrlPnt1 - lineEnd).length();

				// if the point ctrlPnt1 lies above the line(lineStart, lineEnd) => the bezier calculation is extended on 3 more control points
				// Problem: its possible for the rope to bypass this test 
				//				-> needs further testing 
				//				-> maybe not all possibilities were being considered
				if(pointRelationToLine(lineStart, lineEnd, ctrlPnt1) == 1 || (startDistance < 100 || endDistance < 100)){
					Vec3f distanceVector = lineEnd - lineStart;
					Line l = Line(lineStart, distanceVector);
					Vec4f overthrow = pCtrl->overthrow(l, model->getCave(), distanceVector.length());
					if(overthrow[3] != -1){
						ctrlPnt1 = Vec3f(overthrow[0], overthrow[1], overthrow[2]);
						Vec3f direction = hook.getPosition() - ropeOrigin;
						direction.normalize();
						ctrlPnt2 = ctrlPnt1 + direction;
						ctrlPnt3 = ctrlPnt1 - direction;
						ropeHitCave = ropeListSize / 4;
						// initialDirection.normalize();
					}
				}
			}

			lastObj = (* it);
			lastObjPosition = bezierPoint1;
		}
	}
}

void GameController::calculateHookLookAt(){
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

void GameController::calculatePlatformTransition(int p){
	VRGPhysicsObject hook = model->getHook();
	currentPltForm = p;
	ctrlPnt1 = Vec3f(0,0,0);
	Vec3f from = ropeOrigin;
	Vec3f at = hook.getPosition();
	Vec3f lookAt =  at - from;
	hook.setDirection(lookAt);
	MatrixLookAt(from, hook.getPosition() + lookAt, Vec3f(0,1,0), hook.getTransformation()->editRotation());
	changeCurrentState(2);
}

void GameController::calculateRope(){
	VRGPhysicsObject hook = model->getHook();
	Vec3f hookDirection = hook.getDirection();
	Vec3f distanceVector = hook.getPosition() - ropeOrigin;
	Line l = Line(ropeOrigin, distanceVector);
	Vec4f overthrow = pCtrl->overthrow(l, model->getCave(), distanceVector.length());
	if(overthrow[3] != -1 && ctrlPnt1.length() == 0){ // if overthrow[3] != -1 => overthrow[0-2] = hitPoint
		if(ropeHitCave == -1){
			// ropeHitCave = model->getRopeTail().size() / 4;
		}
		ctrlPnt1 = Vec3f(overthrow[0], overthrow[1], overthrow[2]);
		// hookDirection.normalize();
		ctrlPnt2 = ctrlPnt1 + hookDirection;
		ctrlPnt3 = ctrlPnt1 - hookDirection;
	}  else if(overthrow[3] == -1 && (ctrlPnt1.length() != 0 || ropeHitCave > 0)){
		resetBezier();
	} 
	calculateHookLookAt();
	calculateRopePositions();
}

void GameController::animateHookToNewOrigin(){
	VRGPhysicsObject hook = model->getHook();
	resetBezier();
	Vec3f pltformPosition = platforms::positions[currentPltForm] * general::scale;
	Vec3f hookPosition = hook.getPosition();
	Vec3f direction = pltformPosition - hookPosition;
	Vec3f mgrHook = hookPosition - ropeOrigin;
	calculateRopePositions();
	if(direction.length() > physics::minDirectionLengthValue * general::scale){
		Vec3f tempDirection = direction;
		tempDirection.normalize();
		hook.setPosition(hookPosition + tempDirection * 10);
	} else {
		hook.setPosition(pltformPosition);
		mgrHook.normalize();
		hook.setDirection(mgrHook);
		hook.setLookAt(mgrHook);
		MatrixLookAt(hookPosition, hookPosition + mgrHook, Vec3f(0,1,0), hook.getTransformation()->editRotation());
	}
	if(direction.length() < physics::minDirectionLengthValue * general::scale){
		calculateRopePositions();
		changeCurrentState(3);
	}
}

void GameController::callGameLoop(){
	int newTick = calcNewTick();
	if(newTick != currentTick){
		currentTick = newTick;
		VRGPhysicsObject ropeStart = model->getRopeStart();
		VRGPhysicsObject hook = model->getHook();
		Vec3f hookPosition = hook.getPosition();
		Vec3f hookDirection = hook.getDirection();
		float hookDirectionLength = hookDirection.length();
		if(gameState == 1){ 
			if(hookDirectionLength > 0){
				pCtrl->calculateNewTickForPhysicsObject(hook);
				hookDirectionLength = hookDirection.length();
				if(hookDirectionLength > 0){
					bool didHit = pCtrl->collision(hook, model->getCave());
					if(didHit){
						int pltformHit = pCtrl->didHitPlatform(hook, model->getPltformRoot());
						if(pltformHit == currentPltForm + 1){ // Platform getroffen => Seil spannen
							calculatePlatformTransition(pltformHit);
						}
					}
				}
			}
			calculateRope();
			if(!isGrounded()){
				mgr->setTranslation(mgr->getTranslation() - general::upVector * general::scale);
			}
		} else if (gameState == 2){
			animateHookToNewOrigin();
		} else if (gameState == 3){
			if(caveMovement > 0){
				moveTowardsPlattform(caveMovement);
				caveMovement = caveMovement * 0.8;
				if(caveMovement < 0.01)
					caveMovement = 0;
			}
		}
	}
}

void GameController::moveTowardsPlattform(float length){
	if(gameState != 3){ return; }
	Vec3f pltformPosition = platforms::positions[currentPltForm] * general::scale + platforms::offset; //+ pltPositions::offset;
	Vec3f direction = pltformPosition - mgr->getTranslation();
	float dLength = length > direction.length() ? direction.length() : length;
	float dScale = 30;
	dScale = (dScale > dLength ? dLength : dScale);
	direction.normalize();
	if(mgr->getTranslation() != pltformPosition){
		mgr->setTranslation(mgr->getTranslation() + direction * dScale);
	} else {
		ropeOrigin = platforms::positions[currentPltForm];
		mgr->setTranslation(pltformPosition);
		changeCurrentState(1);
		resetGameState(1);
	}
}

void GameController::moveTowardsPlattform(Vec3f movementDirection){
	if(gameState != 3){ return; }
	Vec3f pltformPosition = platforms::positions[currentPltForm] * general::scale + platforms::offset; //+ pltPositions::offset;
	Vec3f direction = pltformPosition - mgr->getTranslation();
	Vec3f normDirection = direction;
	normDirection.normalize();
	Vec3f normMovementDirection = movementDirection;
	normMovementDirection.normalize();
	float dotProd = normDirection * normMovementDirection;
	float length = (1 - dotProd) * movementDirection.length();
	caveMovement += length;
}

int GameController::calcNewTick(){
	std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
	long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - linuxPrevTime).count();
	int newTick = elapsed / (1000 / general::ticksPerSecond);
	if(newTick == 0){
		newTick = currentTick;
	} else {
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


void GameController::init(OSGCSM::CAVESceneManager* newMgr){
	mgr = newMgr;
	pCtrl = new PhysicsController();
	model = new GameModel();
	model->initGameModel(pCtrl);

	currentTick = 0;
	gameState = 1;
	currentPltForm = 0;
	elapsedTime = 0;

	initialDirection = Vec3f(0,0,0);
	ctrlPnt1 = Vec3f(0,0,0);
	ctrlPnt2 = Vec3f(0,0,0);
	ctrlPnt3 = Vec3f(0,0,0);

	caveMovement = 0;
	ropeLength = 0;
	ropeOrigin = platforms::ropeOrigins[currentPltForm] * general::scale;
	linuxPrevTime = std::chrono::steady_clock::now();
}