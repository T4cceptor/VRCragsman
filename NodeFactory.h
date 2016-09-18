
//#define SNIPPET_STEP 7
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4251 4275 4290 4996 4231 4244)
#endif

// OpenSG includes
#pragma once
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGComponentTransform.h>
#include <OpenSG/OSGMaterialGroup.h>
#include <OpenSG/OSGImage.h>
#include <OpenSG/OSGSimpleTexturedMaterial.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGTextureBackground.h>
#include <OpenSG/OSGGradientBackground.h>
#include <OpenSG/OSGPointLight.h>
#include <OpenSG/OSGSpotLight.h>
#include <OpenSG/OSGNavigator.h>
#include <OpenSG/OSGGeoFunctions.h>
#include <OSGTransitPtr.h>
#include <OSGLight.h>
#include <OSGDirectionalLight.h>
#include <OSGPointLight.h>
#include <ctime>
#include <iostream>
#include <math.h>
#include "VRGObject.h"
#include "VRGPhysicsObject.h"
#include "PlattformObject.h"

#ifdef _MSC_VER
# pragma warning(pop)
#endif

OSG_USING_NAMESPACE // activate the OpenSG namespace

class NodeFactory
{
public:
	NodeFactory(void);
	~NodeFactory(void);
	NodeTransitPtr createScenegraph(int type = 1);
	VRGObject createVRGScenegraph(int type = 1);
	int getAFromExternal();

	VRGPhysicsObject createRoot();
	VRGPhysicsObject createCave();
	PlattformObject createPlattform(std::string filePath);

	VRGPhysicsObject createHook();
	VRGPhysicsObject createRope();

	NodeTransitPtr createNewLight(Pnt3f position, NodeRecPtr root);

	VRGObject createEmptyVRGObj();

	VRGPhysicsObject createRopePiece();
};

