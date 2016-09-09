#include "PhysicsController.h"
#include "VRGLogger.h"
#include "Config.h"

extern int currentId;

void testFunction(){

}

PhysicsController::PhysicsController(void)
{
	model = * new PhysicsModel();

	// Werte in Config.h einstellen
	heightDimension = physics::heightDimension;
	floorValue = physics::floorValue * general::scale;
	speed = physics::speed * general::scale;
	speedloss = physics::speedloss * general::scale;
	gravity = physics::gravity * general::scale;
	minDirectionLengthValue = physics::minDirectionLengthValue * general::scale;
	upVector = general::upVector;

	// TODO: funktioniert in der Cave nicht!
	// TODO: in opensg_vrpn.cpp auslagern
	tick = 0;
	time = 0;
	startTime = clock();
}

PhysicsController::~PhysicsController(void)
{
	// physicThread1.kill();
}

void PhysicsController::registerNewPhysicsObject(VRGPhysicsObject obj, bool isMoveable){
	// TODO
	// VRGLogger::logMessage("Registered new physic object ");

	if(isMoveable){
		std::cout << "added new physics object" << std::endl;
		model.addMoveableObject(obj);
	}
}

Vec3f PhysicsController::getReflectionVector(){
	return reflectionVector;
}

// registerNewPhysicsObject
//void PhysicsController::registerNewMoveableObject(VRGPhysicsObject obj){
//	VRGLogger::logMessage("Registered new moveable physic object ");
//}
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
	direction.normalize();
	Line ray = Line(obj1.getPosition() + direction * hook::movementOffsetScale, direction);
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	iAct->setLine(ray, general::hitDistance * general::scale);
	NodeRefPtr someNode = obj2.getRootNode();
	iAct->apply((Node * const)someNode);
    if (iAct->didHit())
    {
      std::cout << "hook hit cave" << std::endl;
		reflectionVector = calcReflectionVector(obj1.getDirection(), iAct->getHitNormal());
		// double dotProd = 1 - abs(iAct->getHitNormal() * Vec3f(0,1,0));
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
		Vec3f itemDirection = obj.getDirection();
		Vec3f itemPosition = obj.getPosition();
		if(itemPosition[heightDimension] > floorValue && itemDirection.length() > 0){ 
			obj.addTranslation(itemDirection * speed); 
			Vec3f newDirection = itemDirection - itemDirection * speedloss;
			newDirection[heightDimension] = newDirection[heightDimension] - gravity;
			obj.setLookAt(newDirection);
			if(newDirection.length() < minDirectionLengthValue){
				newDirection = Vec3f(0,0,0);
			}
			obj.setDirection(newDirection[0],newDirection[1],newDirection[2]);
			MatrixLookAt(itemPosition, itemPosition + obj.getLookAt(), upVector, obj.getTransformation()->editRotation());
		}
}

bool PhysicsController::didHitPLattform(VRGPhysicsObject obj){
	for(int i = 0; i < pltPositions::size; i++){
		Vec3f scaledPosition = pltPositions::positions[i] * general::scale;
		float distance =  (obj.getPosition() - scaledPosition).length();
		if(distance < general::plattformHitDistance){
			return true;
		}
	}
	return false;
}

void PhysicsController::calculateNewTick(){
	clock_t now = clock();
	clock_t delta = now - startTime;
	// int seconds_elapsed = static_cast<int>(delta) / CLOCKS_PER_SEC;
	int newTick = int(static_cast<int>(delta) / 25) % 25;

	//std::cout << "calculating new tick: " << newTick << std::endl;
	//std::cout << "MovableItems: " << model.getMovableItems().size() << std::endl;

	// Simulates movement
	if(newTick != tick && !model.getMovableItems().empty()){
		std::list<VRGPhysicsObject> movObj = model.getMovableItems(); // TODO: Performance upgrade!?
		// besser wenn die items einzeln angesprochen werden?
		// evtl. sogar als static function ? -> !!!
		// Idee: calculateNewTickForObject(VRGPhysicsObject item)
		// => nur einen Hook erzeugen und diesen bewegen! => Performance + Memory Verbesserung
	for (std::list<VRGPhysicsObject>::iterator it = movObj.begin(); it != movObj.end(); ++it){
		VRGPhysicsObject poPtr = (* it);
		calculateNewTickForPhysicsObject(poPtr);

		/*
		Vec3f itemDirection = poPtr.getDirection();
		Vec3f itemPosition = poPtr.getPosition();

		//if(itemPosition[2] > 0 && itemDirection[2] > 0){
		if(itemPosition[heightDimension] > floorValue && itemDirection.length() > 0){ 
			if(tick == 0 && false){
				std::cout << "object direction: " <<  itemDirection[0] << "," << itemDirection[1] << "," << itemDirection[2] << " - " << now << std::endl;
				std::cout << "object position: " <<  itemPosition[0] << "," << itemPosition[1] << "," << itemPosition[2] << " - " << now << std::endl;
			}

			(* it).addTranslation(itemDirection * speed); 
			
			Vec3f newDirection = itemDirection - itemDirection * speedloss;
			newDirection[heightDimension] = newDirection[heightDimension] - gravity;
			if(newDirection.length() < minDirectionLengthValue){
				newDirection = Vec3f(0,0,0);
			}

			poPtr.setDirection(newDirection[0],newDirection[1],newDirection[2]);
			if(tick == 0 && false)
				std::cout << "new object direction: " <<  newDirection[0] << "," << newDirection[1] << "," << newDirection[2] << " - " << tick << std::endl;

			// TODO: Rotation 
			// ComponentTransformRecPtr t = ComponentTransformRecPtr ((* it).getTransformation());
			// t->setRotation(MatrixLookAtResult(t->editMatrix(),itemPosition, itemPosition + itemDirection, Vec3f(0,0,1)));
			MatrixLookAt(itemPosition, itemPosition + itemDirection, upVector, (* it).getTransformation()->editRotation());
			
		}
		*/

		/************************** Old Code ******************************/
			/*Matrix m = Matrix();
			m.setIdentity();
			MatrixLookAtResult(m, itemPosition, itemPosition + itemDirection, Vec3f(0,0,1));
			Quaternion q; 
			Vector3 c = Vector3.Cross(curUp, targetUp); 
			float d = Vector3.Dot(curUp, targetUp); 
			float s = (float) Math.Sqrt((1.0f + d)*2.0f); 
			float rs = 1.0f/s; 
			q.X = c.X*rs; 
			q.Y = c.Y*rs; 
			q.Z = c.Z*rs; 
			q.W = s*0.5f; 
			
			q = q*Rotation; 
			
			Rotation = Quaternion.Slerp(Rotation, q, 0.1f * distanceScalar); 
			*/

		// VRGLogger::logMessage("new tick calculated");
		// std::cout << "before critical point: if" << std::endl;

		// TODO: Hit-Box check!
		// if(needCollisionCheck && !didHitAnything((* it))){
		//if(float((* it).getPosition().getValues()[2]) > -10.0f){ // TODO: -10 wird nicht der richtige Wert sein!
		//	(* it).setDirection(itemDirection.getValues()[0], itemDirection.getValues()[1], itemDirection.getValues()[2] - 1);
		//	// TODO: direction �ndern, Schwerkraft
		//	// hier wird z value der Direction ge�ndert
		//}

		/*
		if((* it).speed > 0.5)
			(* it).setSpeed((* it).speed * 0.5);
		else
			(* it).setSpeed(0);
		*/
		// } else {
		// TODO
		// }
		// itemDirection = 0;
	}
	tick = newTick;
	}
}
