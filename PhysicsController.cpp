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
		Vec3f view = at - from;
		view.normalize();
		Vec3f right = up % view;
		right.normalize();
		Vec3f newup = view % right;
		Vec3f objForward = Vec3f(0,0,1);
		Vec3f objUp = Vec3f(0,1,0);

		float dot2 =  right * objForward;
		Vec3f newView = Vec3f(view[0],0,view[2]);
		newView.normalize();
		float realDotXYPlane = newView * objForward;
		float realAngle = acos(realDotXYPlane);
		Quaternion q1 = Quaternion(objUp, dot2 > 0 ? -realAngle : realAngle);

		// rotation = q1;
		float dot3 = newup * objUp;
		float angle2 = 0.0f;
		if(abs(abs(dot3) - 1.0f) > 0.0001f){
			angle2 = acos(dot3);
		}

		float dot4 = view * objUp;
		Quaternion q2 = Quaternion(Vec3f(-1,0,0), dot4 < 0 ? -angle2 : angle2);
		rotation = q1 * q2;
}

bool PhysicsController::collision(VRGPhysicsObject& obj1, VRGPhysicsObject& obj2){
	// TODO
	Line ray = Line(obj1.getPosition() + obj1.getDirection(), obj1.getDirection());
	// std::cout << "from: " << obj1.getPosition() << " to: " << obj1.getPosition() + obj1.getDirection() << std::endl;
	std::cout << "vector: " << ray << std::endl;
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	iAct->setLine(ray, general::hitDistance);

	NodeRefPtr someNode = obj2.getRootNode();
	iAct->apply((Node * const)someNode);
    if (iAct->didHit())
    {
		reflectionVector = calcReflectionVector(obj1.getDirection(), iAct->getHitNormal());
		double dotProd = 1 - abs(iAct->getHitNormal() * Vec3f(0,1,0));
		// obj1.setDirection(obj1.getDirection() * (dotProd > 0.1 ? dotProd : 0.1));
		// std::cout << "new dotProd: " << dotProd << std::endl;
		return true;
    }
	return false;
}

Vec3f PhysicsController::calcReflectionVector(Vec3f direction,Vec3f normal){
	// TODO: nicht korrekt??
	// R= 2*(-I dot N)*N + I 
	Vec3f tempDirection = Vec3f(direction);
	float speed = direction.length();
	tempDirection.normalize();
	normal.normalize();
	// double dotProd = tempDirection * normal;

	/*
	R ist der neue Richtungsvektor
I ist der alte Richtungsvektor vor der Kollision
N ist der Normalenvektor des Kollisionspunktes 

Der neue Vektor R wird wie folgt berechnet: 

R= 2*(-I dot N)*N + I 
	*/

	Vec3f r = 2 * (-direction.dot(normal)) * normal + direction;
	r.normalize();
	return r * speed * cave::velocityReduction;
}

/*
TVector ReflectVector (TVector a, TVector norm) 
{
	TVector res;
//	NormalizeVector (&a);
//	NormalizeVector (&norm);
	double dotprod = DotProduct (a, norm);
	res.x = a.x - 2 * norm.x * dotprod;
	res.y = a.y - 2 * norm.y * dotprod;
	res.z = a.z - 2 * norm.z * dotprod;
	return res;
}
*/

void PhysicsController::calculateNewTickForPhysicsObject(VRGPhysicsObject& obj){
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
		//	// TODO: direction ändern, Schwerkraft
		//	// hier wird z value der Direction geändert
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
