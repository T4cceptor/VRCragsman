#include "PhysicsController.h"
#include "VRGLogger.h"
#include "Config.h"

extern int currentId;
extern float timeDelta;

PhysicsController::PhysicsController(void)
{
	model = * new PhysicsModel();
	std::cout << "PhysicsController contructor called" << std::endl;

	// Werte in Config.h einstellen
	heightDimension = physics::heightDimension;
	floorValue = physics::floorValue * general::scale;
	speed = physics::speed * general::scale;
	speedloss = physics::speedloss * general::scale;
	gravity = physics::gravity * general::scale;
	minDirectionLengthValue = physics::minDirectionLengthValue * general::scale;
	upVector = general::upVector;

	tick = 0;
	time = 0;
	startTime = clock();
}

PhysicsController::~PhysicsController(void){}

// deprecated - use "calculateNewTickForPhysicsObject" instead
//void PhysicsController::registerNewPhysicsObject(VRGPhysicsObject obj, bool isMoveable){
//	// TODO
//	// VRGLogger::logMessage("Registered new physic object ");
//	if(isMoveable){
//		std::cout << "added new physics object" << std::endl;
//		model.addMoveableObject(obj);
//	}
//}

Vec3f PhysicsController::getReflectionVector(){
	return reflectionVector;
}

void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation){
		// TODO Performance upgrade!!!
//		std::cout << "calculating new look at: " << at << std::endl;
//		std::cout << "rotation: " << rotation << std::endl;
		Vec3f view = at - from;
		view.normalize();
		Vec3f right = up % view;
		right.normalize();
		Vec3f newup = view % right;
		Vec3f objForward = Vec3f(0,0,1);
		Vec3f objUp = general::upVector;
		float dot2 =  right * objForward;
		Vec3f newView = Vec3f(view[0],0,view[2]);
		newView.normalize();
		float realDotXYPlane = newView * objForward;
		float realAngle = acos(realDotXYPlane);
		Quaternion q1 = Quaternion(objUp, dot2 > 0 ? -realAngle : realAngle);
		float dot3 = newup * objUp;
		float angle2 = 0.0f;
		if(abs(abs(dot3) - 1.0f) > 0.0001f){
			angle2 = acos(dot3);
		}
		float dot4 = view * objUp;
		Quaternion q2 = Quaternion(Vec3f(-1,0,0), dot4 < 0 ? -angle2 : angle2);
		rotation = q1 * q2;
}

bool PhysicsController::collision(VRGPhysicsObject obj1, VRGPhysicsObject obj2){
	Vec3f direction = obj1.getDirection();
	float directionLength = direction.length() * speed;
	direction.normalize();
	Line ray = Line(obj1.getPosition(), direction);
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	iAct->setLine(ray, directionLength); 
	NodeRefPtr someNode = obj2.getRootNode();
	iAct->apply((Node * const)someNode);
    if (iAct->didHit())
    {
		reflectionVector = calcReflectionVector(obj1.getDirection(), iAct->getHitNormal());
		obj1.setDirection(reflectionVector);
		Vec3f hitPoint = iAct->getHitPoint().subZero();
		obj1.setPosition(hitPoint[0],hitPoint[1],hitPoint[2]);
		return true;
    }
	return false;
}

Vec3f PhysicsController::calcReflectionVector(Vec3f direction,Vec3f normal){
	Vec3f tempDirection = Vec3f(direction);
	float speed = direction.length();
	tempDirection.normalize();
	normal.normalize();
	Vec3f r = 2 * (-direction.dot(normal)) * normal + direction;
	r.normalize();
	return r * speed * cave::velocityReduction;
}

void PhysicsController::calculateNewTickForPhysicsObject(VRGPhysicsObject obj){
	// TODO: zeitliche Komponente mit einbeziehen
		Vec3f itemDirection = obj.getDirection();
		Vec3f itemPosition = obj.getPosition();
		if(itemPosition[heightDimension] > floorValue && itemDirection.length() > 0){ 
			obj.addTranslation(itemDirection * speed); 
			Vec3f newDirection = itemDirection - itemDirection * speedloss;
			newDirection[heightDimension] = newDirection[heightDimension] - gravity;
			
			if(newDirection.length() < minDirectionLengthValue){
				newDirection = Vec3f(0,0,0);
			} else {
				// obj.setLookAt(newDirection);
			} 
			obj.setDirection(newDirection[0],newDirection[1],newDirection[2]);
			MatrixLookAt(itemPosition, itemPosition + obj.getLookAt(), upVector, obj.getTransformation()->editRotation());
		}
}

int PhysicsController::didHitPLattform(VRGPhysicsObject obj){
	for(int i = 0; i < pltPositions::size; i++){
		Vec3f scaledPosition = pltPositions::positions[i] * general::scale;
		float distance =  (obj.getPosition() - scaledPosition).length();
		if(abs(obj.getPosition()[1] - scaledPosition[1]) < general::plattformHitDistance * general::scale / 5 && distance < general::plattformHitDistance * general::scale){
			return i;
		}
	}
	return -1;
}

// deprecated - use "calculateNewTickForPhysicsObject" instead
//void PhysicsController::calculateNewTick(){
//	clock_t now = clock();
//	clock_t delta = now - startTime;
//
//	int newTick = int(static_cast<int>(delta) / 25) % 25;
//	if(newTick != tick && !model.getMovableItems().empty()){
//		std::list<VRGPhysicsObject> movObj = model.getMovableItems(); // TODO: Performance upgrade!?
//		// besser wenn die items einzeln angesprochen werden?
//		// evtl. sogar als static function ? -> !!!
//		// Idee: calculateNewTickForObject(VRGPhysicsObject item)
//		// => nur einen Hook erzeugen und diesen bewegen! => Performance + Memory Verbesserung
//	for (std::list<VRGPhysicsObject>::iterator it = movObj.begin(); it != movObj.end(); ++it){
//		VRGPhysicsObject poPtr = (* it);
//		calculateNewTickForPhysicsObject(poPtr);
//
//		/*
//		Vec3f itemDirection = poPtr.getDirection();
//		Vec3f itemPosition = poPtr.getPosition();
//
//		//if(itemPosition[2] > 0 && itemDirection[2] > 0){
//		if(itemPosition[heightDimension] > floorValue && itemDirection.length() > 0){ 
//			if(tick == 0 && false){
//				std::cout << "object direction: " <<  itemDirection[0] << "," << itemDirection[1] << "," << itemDirection[2] << " - " << now << std::endl;
//				std::cout << "object position: " <<  itemPosition[0] << "," << itemPosition[1] << "," << itemPosition[2] << " - " << now << std::endl;
//			}
//
//			(* it).addTranslation(itemDirection * speed); 
//			
//			Vec3f newDirection = itemDirection - itemDirection * speedloss;
//			newDirection[heightDimension] = newDirection[heightDimension] - gravity;
//			if(newDirection.length() < minDirectionLengthValue){
//				newDirection = Vec3f(0,0,0);
//			}
//
//			poPtr.setDirection(newDirection[0],newDirection[1],newDirection[2]);
//			if(tick == 0 && false)
//				std::cout << "new object direction: " <<  newDirection[0] << "," << newDirection[1] << "," << newDirection[2] << " - " << tick << std::endl;
//
//			// TODO: Rotation 
//			// ComponentTransformRecPtr t = ComponentTransformRecPtr ((* it).getTransformation());
//			// t->setRotation(MatrixLookAtResult(t->editMatrix(),itemPosition, itemPosition + itemDirection, Vec3f(0,0,1)));
//			MatrixLookAt(itemPosition, itemPosition + itemDirection, upVector, (* it).getTransformation()->editRotation());
//			
//		}
//		*/
//
//		/************************** Old Code ******************************/
//			/*Matrix m = Matrix();
//			m.setIdentity();
//			MatrixLookAtResult(m, itemPosition, itemPosition + itemDirection, Vec3f(0,0,1));
//			Quaternion q; 
//			Vector3 c = Vector3.Cross(curUp, targetUp); 
//			float d = Vector3.Dot(curUp, targetUp); 
//			float s = (float) Math.Sqrt((1.0f + d)*2.0f); 
//			float rs = 1.0f/s; 
//			q.X = c.X*rs; 
//			q.Y = c.Y*rs; 
//			q.Z = c.Z*rs; 
//			q.W = s*0.5f; 
//			
//			q = q*Rotation; 
//			
//			Rotation = Quaternion.Slerp(Rotation, q, 0.1f * distanceScalar); 
//			*/
//
//		// VRGLogger::logMessage("new tick calculated");
//		// std::cout << "before critical point: if" << std::endl;
//
//		// TODO: Hit-Box check!
//		// if(needCollisionCheck && !didHitAnything((* it))){
//		//if(float((* it).getPosition().getValues()[2]) > -10.0f){ // TODO: -10 wird nicht der richtige Wert sein!
//		//	(* it).setDirection(itemDirection.getValues()[0], itemDirection.getValues()[1], itemDirection.getValues()[2] - 1);
//		//	// TODO: direction �ndern, Schwerkraft
//		//	// hier wird z value der Direction ge�ndert
//		//}
//
//		/*
//		if((* it).speed > 0.5)
//			(* it).setSpeed((* it).speed * 0.5);
//		else
//			(* it).setSpeed(0);
//		*/
//		// } else {
//		// TODO
//		// }
//		// itemDirection = 0;
//	}
//	tick = newTick;
//	}
//}
