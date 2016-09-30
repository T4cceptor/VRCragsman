#pragma once
#include "VRGObject.h"
#include <OSGComponentTransform.h>
#include <OSGDirectionalLight.h>
#include <OSGSpotLight.h>
#include <OSGVector.h>
#include <OSGNode.h>

OSG_USING_NAMESPACE

class LightWrapper
{
public:
	LightWrapper(void);
	~LightWrapper(void);

	LightTransitPtr getLight();
	NodeTransitPtr getRootNode();

	ComponentTransformTransitPtr getTransformation();

	Vec3f getPosition();
	void setPosition(Vec3f newPosition);

	Vec3f getDirection();
	void setDirection();

	void addChild(NodeRecPtr child);
	void addChild(VRGObject obj);

	void addTranslation(Vec3f newPosition);

private:
	LightBase * light;
};

