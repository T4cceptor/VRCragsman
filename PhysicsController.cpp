#include "PhysicsController.h"
#include "VRGLogger.h"
#include "Config.h"

extern int currentId;
extern float timeDelta;

PhysicsController::PhysicsController(void)
{
	std::cout << "PhysicsController contructor called" << std::endl;

	// Werte in Config.h einstellen
	heightDimension = physics::heightDimension;
	floorValue = physics::floorValue * general::scale;
	speed = physics::speed * general::scale;
	speedloss = physics::speedloss * general::scale;
	gravity = physics::gravity * general::scale;
	minDirectionLengthValue = physics::minDirectionLengthValue * general::scale;
	upVector = general::upVector;
}

PhysicsController::~PhysicsController(void){}

Vec3f PhysicsController::getReflectionVector(){
	return reflectionVector;
}

void MatrixLookAt(OSG::Pnt3f from, OSG::Pnt3f at, OSG::Vec3f up, OSG::Quaternion& rotation){
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

bool PhysicsController::collision(VRGPhysicsObject obj1, VRGObject obj2){
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

Vec4f PhysicsController::overthrow(Line line, VRGObject obj, float length = -1){
	Vec4f result = Vec4f(0,0,0,-1);
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	if(length == -1)
		iAct->setLine(line); 
	else
		iAct->setLine(line, length); 
	NodeRefPtr someNode = obj.getRootNode();
	iAct->apply((Node * const)someNode);
    if (iAct->didHit())
	{
		Vec3f hit = iAct->getHitPoint().subZero();
		result = Vec4f(hit[0],hit[1],hit[2],1);
	}
	return result;
}

Vec3f PhysicsController::calcReflectionVector(Vec3f direction,Vec3f normal){
	Vec3f tempDirection = Vec3f(direction);
	float speed = direction.length();
	tempDirection.normalize();
	normal.normalize();
	Vec3f r = 2 * (-direction.dot(normal)) * normal + direction;
	r.normalize();
	return r * speed * cave::velocityReduction - cave::gravity;
}

void PhysicsController::calculateNewTickForPhysicsObject(VRGPhysicsObject obj){
		Vec3f itemDirection = obj.getDirection();
		Vec3f itemPosition = obj.getPosition();
		if(itemPosition[heightDimension] > floorValue && itemDirection.length() > 0){ 
			obj.addTranslation(itemDirection * speed); 
			Vec3f newDirection = itemDirection - itemDirection * speedloss;
			newDirection[heightDimension] = newDirection[heightDimension] - gravity;
			if(newDirection.length() < minDirectionLengthValue){
				newDirection = Vec3f(0,0,0);
			} 
			obj.setDirection(newDirection[0],newDirection[1],newDirection[2]);
			MatrixLookAt(itemPosition, itemPosition + obj.getLookAt(), upVector, obj.getTransformation()->editRotation());
		}
}

int PhysicsController::didHitPlatform(VRGPhysicsObject obj, VRGObject pltformRoot){
	// Vec3f direction = obj.getDirection();
	Vec3f direction = Vec3f(0, -40, 0);
	float directionLength = direction.length();
	direction.normalize();
	Line ray = Line(obj.getPosition(), direction);
	IntersectActionRefPtr iAct = (IntersectActionRefPtr)IntersectAction::create();
	iAct->setLine(ray, directionLength); 
	NodeRefPtr someNode = pltformRoot.getRootNode();
	iAct->apply((Node * const)someNode);
	int pltformIndex = -1;
	float distance = 0;
    if (iAct->didHit())
    {
		float minDistance = 10000;
		Vec3f hitPoint = (iAct->getHitPoint()).subZero();
		for(int i = 0; i < platforms::count; i++){
			Vec3f scaledPosition = platforms::positions[i] * general::scale;
				distance =  ( hitPoint - scaledPosition ).length();
				if( distance < minDistance ){
					minDistance = distance;
					pltformIndex = i;
				}
		}
    }
	return pltformIndex;
}