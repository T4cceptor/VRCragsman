#pragma once
#include <OSGNode.h>
#include <OSGComponentTransform.h>

OSG_USING_NAMESPACE

class VRGObject
{
public:
	VRGObject(void);
	VRGObject(int id);
	VRGObject(NodeRecPtr root);
	VRGObject(NodeRecPtr root, int id);
	virtual ~VRGObject(void);

	void setGeometryNode(NodeRecPtr newGeoNode);
	void setTransformation(ComponentTransformRecPtr newTransform);
	void setRootNode(NodeRecPtr newRoot);

	void addTranslation(Vec3f newPosition);

	NodeTransitPtr getRootNode();
	NodeTransitPtr getGeoNode();
	ComponentTransformTransitPtr getTransformation();

	void addChild(NodeRecPtr child);
	void addChild(VRGObject obj);

	Quaternion getRotation();
	Vec3f getPosition();
	void setPosition(float x, float y, float z);
	void setPosition(Vec3f newPosition);

	int getId();

private:
	int id;
	NodeRecPtr root;
	NodeRecPtr geoNode;
	ComponentTransformRecPtr objectTransformation;

	/* TODO - ?
		ObjectNode -> ObjectCore
		TransformationNode
		TextureNode
		MaterialNode
	*/
	// Vec3f position;
	
};

