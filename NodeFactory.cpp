#include "NodeFactory.h"
#include "Config.h"

/* * * * * * * TODO * * * * * * * * *
- createNode sinnvoll verallgemeinern - ?

*/


NodeFactory::NodeFactory(void){}

NodeFactory::~NodeFactory(void){}

NodeTransitPtr createNodeFromFile(NodeRecPtr root, std::string filePath){
	NodeRecPtr cave = SceneFileHandler::the()->read(filePath.c_str());
	root->addChild(cave);
	return NodeTransitPtr(root);
}

NodeTransitPtr createNodeFromFile(std::string filePath){
	NodeRecPtr cave = SceneFileHandler::the()->read(filePath.c_str());
	return NodeTransitPtr(cave);
}

PlattformObject NodeFactory::createPlattform(std::string filePath){

	PlattformObject returnObj = * new PlattformObject();
	NodeRecPtr plattforms = createNodeFromFile(filePath);
	returnObj.setGeometryNode(plattforms);
	// Plattform1Only
	GeometryRecPtr geo = dynamic_cast<Geometry *>(plattforms->getCore());
	createSharedIndex(geo);
	// calcFaceNormals(geo);
	calcVertexNormals(geo);

	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);

	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(plattforms);
	returnObj.setRootNode(transNode);
	return returnObj;
}

VRGPhysicsObject NodeFactory::createCave(){
	// TODO init cave attributes
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();
	NodeRecPtr cave = createNodeFromFile(path::caveModel);
	returnObj.setGeometryNode(cave);
	// GeometryRecPtr geo = cave->getCore();
	// GeometryRecPtr geo = dynamic_cast<Geometry *>(scene->getCore());
	GeometryRecPtr geo = dynamic_cast<Geometry *>(cave->getCore());
	createSharedIndex(geo);
	// calcFaceNormals(geo);
	calcVertexNormals(geo);
    // calcVertexNormals(geo, osgDegree2Rad(30));

	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);

	// TODO: implement scaling factor
	compTransform->setScale(cave::scaleVector * general::scale);
    compTransform->setTranslation(cave::translationVector * general::scale);

	// TODO
	// compTransform->setCenter
	// compTransform->setMatrix
	// bt->setRotation(Quaternion(Vec3f(1,0,0),osgDegree2Rad(270)+0.001f*time));
	// compTransform->setRotation(Quaternion(Vec3f(1,0,0),osgDegree2Rad(0)));
	// compTransform->setScale
	// compTransform->setTranslation

	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(cave);
	returnObj.setRootNode(transNode);
	return returnObj;
}

VRGObject NodeFactory::createEmptyVRGObj(){
	VRGObject returnObj = * new VRGObject();
	NodeRecPtr root = Node::create();
	root->setCore(Group::create());
	returnObj.setRootNode(root);
	return returnObj;
}

VRGPhysicsObject NodeFactory::createRoot(){
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();
	NodeRecPtr root = Node::create();
	root->setCore(Group::create());
	returnObj.setRootNode(root);
	return returnObj;
}

NodeTransitPtr NodeFactory::createNewLight(Pnt3f position, NodeRecPtr root){

	NodeRecPtr returnPtr = Node::create();
	returnPtr->setCore(Group::create());

	NodeRecPtr lightPosition = Node::create();
	ComponentTransformRecPtr ct = ComponentTransform::create();
	ct->setTranslation(position.subZero());
	lightPosition->setCore(ct);
	returnPtr->addChild(lightPosition);

	
	PointLightRecPtr sunLight = PointLight::create();
	//sunLight->setAttenuation(1,0,2);

	Color4f globalDiffuse = Color4f(0.6,0.6,0.6,1);
	Color4f globalAmbient = Color4f(0.2f,0.2f,0.2f,1);
	Color4f globalSpecular = Color4f(0.8,0.8,0.8,1);

	/*
	sunLight->setDiffuse(Color4f(1,1,1,1));
	sunLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	sunLight->setSpecular(Color4f(1,1,1,1));

	sunLight->setDiffuse(Color4f(0.2,0.3,0.5,1));
	sunLight->setAmbient(Color4f(0.3f,0.3f,0.3f,1));
	sunLight->setSpecular(Color4f(0.5,0.6,0.8,1));
	*/

	sunLight->setDiffuse(globalDiffuse);
	sunLight->setAmbient(globalAmbient);
	sunLight->setSpecular(globalSpecular);
	sunLight->setBeacon(lightPosition);

	NodeRecPtr sunLightNode = Node::create();
	sunLightNode->setCore(sunLight);
	returnPtr->addChild(sunLightNode);
	root->addChild(returnPtr);

	return NodeTransitPtr(sunLightNode);
	/*
	ComponentTransformRecPtr ct = ComponentTransform::create();
	ct->setTranslation(Vec3f(0,-2,0));
	ct->setRotation(Quaternion(Vec3f(1,0,0),osgDegree2Rad(90)));
	/*
	beachTrans = Node::create();
	beachTrans->setCore(ct);

	PointLightRecPtr sunLight = PointLight::create();
	//sunLight->setAttenuation(1,0,2);

	//color information
	sunLight->setDiffuse(Color4f(1,1,1,1));
	sunLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	sunLight->setSpecular(Color4f(1,1,1,1));

	sunLight->setBeacon(sunChild); //attach to the sun node use this node as position beacon

	root->setCore(sunLight);
	*/

	/*
		DirectionalLightRecPtr dirLight = DirectionalLight::create();
	dirLight->setDirection(1,1,-1);

	//color information
	dirLight->setDiffuse(Color4f(1,1,1,1));
	dirLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	dirLight->setSpecular(Color4f(1,1,1,1));


	//wrap the root, cause only nodes below the lights will be lit
	NodeRecPtr ueberroot = makeNodeFor(dirLight);
	ueberroot->addChild(root);

	*/

}

VRGPhysicsObject NodeFactory::createHook(){
	// TODO init cave attributes
	VRGPhysicsObject returnObj = * new VRGPhysicsObject();

	// TODO: auf .obj file umstellen
	NodeRecPtr hook = createNodeFromFile(path::hookModel);
	returnObj.setGeometryNode(hook);
	ComponentTransformRecPtr compTransform = ComponentTransform::create();
	returnObj.setTransformation(compTransform);
	// compTransform->setTranslation(Vec3f(100,20,100));

	// TODO
	// compTransform->setCenter
	// compTransform->setMatrix
	// compTransform->setRotation
	// compTransform->setScale
	// compTransform->setTranslation

	NodeRecPtr transNode = makeNodeFor(compTransform);
	transNode->addChild(hook);
	returnObj.setRootNode(transNode);
	return returnObj;
}



/*
NodeTransitPtr loadCave(NodeRecPtr root, std::string filePath){ // "models/vrcave2.3ds"
	ComponentTransformRecPtr caveTransform = ComponentTransform::create();
	NodeRecPtr palmTrans = makeNodeFor(caveTransform);
	createNodeFromFile(palmTrans, filePath);

	// TODO cave richtig aufstellen

	return NodeTransitPtr(palmTrans);
}
*/

/*
NodeTransitPtr loadHook(NodeRecPtr root, std::string filePath){ // "models/hookv2.3ds"
	ComponentTransformRecPtr hookTransform = ComponentTransform::create();
	NodeRecPtr hookTransNode = makeNodeFor(hookTransform);
	createNodeFromFile(hookTransNode, filePath);
	return NodeTransitPtr(hookTransNode);
}
*/

/*
NodeTransitPtr createScene1(){
	NodeRecPtr groupNode1 = Node::create();
	groupNode1->setCore(Group::create());

	NodeRecPtr boxChild = makeBox(5,4,4,1,1,1);
	NodeRecPtr beach = makePlane(30, 30, 1, 1);
	NodeRecPtr teapot = makeTeapot(10, 2.0);

	GeometryRecPtr sunGeo = makeSphereGeo(2, 3);
	NodeRecPtr sunChild = Node::create();
	sunChild->setCore(sunGeo);

	groupNode1->addChild(sunChild);
	groupNode1->addChild(boxChild);
	groupNode1->addChild(beach);
	groupNode1->addChild(teapot);

	return NodeTransitPtr(groupNode1);
}
*/

/* old code
NodeTransitPtr NodeFactory::createNewType1Scenegraph() {
	// Create root node for the scenegraph
	NodeRecPtr root = Node::create();
	root->setCore(Group::create());

	/*----------1.1----------
	NodeRecPtr boxChild = makeBox(5,4,4,1,1,1);
	NodeRecPtr beach = makePlane(30, 30, 1, 1);
	NodeRecPtr teapot = makeTeapot(10, 2.0);
	GeometryRecPtr sunGeo = makeSphereGeo(2, 3);
	NodeRecPtr sunChild = Node::create();
	sunChild->setCore(sunGeo);
	root->addChild(sunChild);
	root->addChild(boxChild);
	root->addChild(beach);
	root->addChild(teapot);
	//root->addChild(createScene1());
	/*----------2.1----------
	//decouple the nodes to be shifted in hierarchy from the scene
	root->subChild(sunChild);
	root->subChild(beach);
	TransformRecPtr sunTransCore = Transform::create();
	Matrix sunMatrix;
	// Setting up the matrix
	sunMatrix.setIdentity();
	sunMatrix.setTranslate(0,20,0);
	sunTransCore->setMatrix(sunMatrix); // Adding the Matrix to the core
	// Setting up the node
	NodeRecPtr sunTrans = makeNodeFor(sunTransCore);
	sunTrans->addChild(sunChild);
	ComponentTransformRecPtr ct = ComponentTransform::create();
	ct->setTranslation(Vec3f(0,-2,0));
	ct->setRotation(Quaternion(Vec3f(1,0,0),osgDegree2Rad(90)));
	/*
	beachTrans = Node::create();
	beachTrans->setCore(ct);
	beachTrans->addChild(beach);
	
	moveNode(teapot, Vec3f(10,10,10));
	// put the nodes in the scene again
	//root->addChild(beachTrans);
	root->addChild(sunTrans);
	/*----------3.1----------
	root->subChild(sunTrans);
	SimpleMaterialRecPtr sunMat = SimpleMaterial::create();
	sunMat->setDiffuse(Color3f(1,0.8f,0));
	sunMat->setAmbient(Color3f(0.8f, 0.2f, 0.2f));
	MaterialGroupRecPtr sunMgCore = MaterialGroup::create();
	sunMgCore->setMaterial(sunMat);
	NodeRecPtr sunMg = Node::create();
	sunMg->setCore(sunMgCore);
	sunMg->addChild(sunTrans);
	root->addChild(sunMg);
	SimpleMaterialRecPtr boxMat = SimpleMaterial::create();
	boxMat->setDiffuse(Color3f(1,0.2f,0.1f));
	boxMat->setAmbient(Color3f(0.8f, 0.2f, 0.2f));
	boxMat->setTransparency(0.25);
	//boxMat->setLit(false);
	GeometryRecPtr boxGeo = dynamic_cast<Geometry*>(boxChild->getCore());
	boxGeo->setMaterial(boxMat);
	setColorForNode(teapot, 0.5f, 1.0f, 0.1f, 0.0f);

	/*----------4.1----------
	/*
	ImageRecPtr image = Image::create();
	// sand taken from http://www.filterforge.com/filters/720.jpg
	image->read("models/sand.jpg");

	//now we create the texture that will hold the image
	SimpleTexturedMaterialRecPtr tex = SimpleTexturedMaterial::create();
	tex->setImage(image);

	//now assign the fresh texture to the geometry
	
	GeometryRecPtr beachGeo = dynamic_cast<Geometry*>(beach->getCore());
	beachGeo->setMaterial(tex);
	

	/*----------5.1----------
	root->addChild( loadCave(root, "models/vrcave2.3ds") ); // loadCave( rootNode , fileName );
	root->addChild( loadHook(root, "models/hookv2.3ds") ); // loadHook( rootNode , fileName );

	/*----------7.1 Light ----------
	PointLightRecPtr sunLight = PointLight::create();
	//sunLight->setAttenuation(1,0,2);

	//color information
	sunLight->setDiffuse(Color4f(1,1,1,1));
	sunLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	sunLight->setSpecular(Color4f(1,1,1,1));

	sunLight->setBeacon(sunChild); //attach to the sun node use this node as position beacon

	root->setCore(sunLight);

	DirectionalLightRecPtr dirLight = DirectionalLight::create();
	dirLight->setDirection(1,1,-1);

	//color information
	dirLight->setDiffuse(Color4f(1,1,1,1));
	dirLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	dirLight->setSpecular(Color4f(1,1,1,1));

	//wrap the root, cause only nodes below the lights will be lit
	NodeRecPtr ueberroot = makeNodeFor(dirLight);
	ueberroot->addChild(root);

	root = ueberroot;

	return NodeTransitPtr(root);
}
*/