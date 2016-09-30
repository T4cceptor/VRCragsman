#include "OSGVector.h"
#include <string.h>
#include "OSGColor.h"

namespace config{
	const int test = 0;
}

namespace path{
	const std::string caveModel = "../models/Cave3.obj";
	const std::string hookModel = "../models/hookwithknot3.obj";
	const std::string ropeModel = "../models/ropeonly.obj";
	const std::string anchorModel = "../models/Anker.obj";
}

namespace configLight{
	
	const Color4f globalDiffuse = Color4f(0.6,0.6,0.6,1);
	const Color4f globalAmbient = Color4f(0.45f,0.45f,0.45f,1);
	const Color4f globalSpecular = Color4f(0.3,0.3,0.3,1);

	/*
	sunLight->setDiffuse(Color4f(1,1,1,1));
	sunLight->setAmbient(Color4f(0.2f,0.2f,0.2f,1));
	sunLight->setSpecular(Color4f(1,1,1,1));

	sunLight->setDiffuse(Color4f(0.2,0.3,0.5,1));
	sunLight->setAmbient(Color4f(0.3f,0.3f,0.3f,1));
	sunLight->setSpecular(Color4f(0.5,0.6,0.8,1));
	*/

	const int positionsSize = 5;
	const Vec3f positions[positionsSize] = { 
		Vec3f(-20000,-3000,-15000),
		Vec3f( -30, 500, -40),
		Vec3f(-1040,-2500,-540),
		Vec3f(-1540,-2500,-740),
		Vec3f(-2040,-2500,-940),
		//Vec3f(-1640,-2500,-1140),
		//Vec3f(-1840,-2500,-1340),
		//Vec3f(-2040,-2500,-1540),
		//Vec3f(-2240,-2500,-1740),
		//Vec3f(-2440,-2500,-1940),
	};

	// l2->setAttenuation(3,0.001,0);
	/*
	Color4f globalDiffuse = Color4f(0.4,0.4,0.6,0.5);
	Color4f globalAmbient = Color4f(0.1f,0.3f,0.3f,0.5);
	Color4f globalSpecular = Color4f(0.3,0.3,0.3,0.5);
	*/
	const Vec3f attenuations[positionsSize] = {
		Vec3f (2,0.00001,0),
		Vec3f (1,0.00002,0),
		//Vec3f (2,0.0002,0),
		//Vec3f (3,0.0001,0),
		//Vec3f (3,0.0001,0)
		Vec3f (20,0.02,0),
		Vec3f (30,0.01,0),
		Vec3f (30,0.01,0)
	};
	const Vec4f diffuseColors[positionsSize] = {
		Vec4f (0,0,0,0), 
		Vec4f (0.6,0.6,0.8,1),
		Vec4f (0.4,0.4,0.6,0.5),
		Vec4f (0.4,0.4,0.6,0.5),
		Vec4f (0.4,0.4,0.6,0.5)
	};
	const Vec4f ambientColors[positionsSize] = {
		Vec4f (0,0,0,0), // R: 255, G: 255, B: 204
		Vec4f (0.05f,0.15f,0.15f,1),
		Vec4f (0.1f,0.3f,0.3f,0.5),
		Vec4f (0.1f,0.3f,0.3f,0.5),
		Vec4f (0.1f,0.3f,0.3f,0.5)
	};
	
	
	const Vec4f specularColors[positionsSize] = {
		Vec4f (0,0,0,0), // R: 255, G: 153, B: 51
		Vec4f (0.1,0.1,0.1,1),
		Vec4f (0.3,0.3,0.3,0.5),
		Vec4f (0.3,0.3,0.3,0.5),
		Vec4f (0.3,0.3,0.3,0.5)
	};
}

namespace general {
	const double scale = 5.5; // muss entsprechend der Ausf�hrungsumgebung angepasst werden!
	const double movementFactor = 10.0;

	const Vec3f upVector = Vec3f(0,1,0); // je nach Koordinatensystem einstellen
	const Vec3f rightVector = Vec3f(1,0,0); // je nach Koordinatensystem einstellen
	const Vec3f forwardVector = Vec3f(0,0,-1); // je nach Koordinatensystem einstellen

	const float plattformHitDistance = 300.0f;
	const float minDistanceToFloor = 50.0f;
	const float hitDistance = 20.0f;
	const int ticksPerSecond = 50;

	const int maxCount = 100;

	const double pi = atan(1)*4;
}

namespace cave{
	const Vec3f translationVector = Vec3f(370,0,330); // Anf�nglicher Translations Vektor
	const Vec3f scaleVector = Vec3f(10,10,10); // Skalierung des 3D Models
	const float velocityReduction = 0.2; // gibt Prozentwert an, auf den die Geschwindigkeit eines auftreffenden Objekts veringert wird
	const Vec3f gravity = Vec3f(0,-3, 0);
}

namespace anchor{
	const Vec3f translationVector = Vec3f(0,0,0); // Anf�nglicher Translations Vektor
	const Vec3f scaleVector = Vec3f(20,20,20); // Skalierung des 3D Models
}

namespace hook{
	const Vec3f translationVector = Vec3f(0,0,0); // Anf�nglicher Translations Vektor
	const float scale = 8;
	const Vec3f scaleVector = Vec3f(1,1,1) * scale; // Skalierung des 3D Models

	const float movementOffsetScale = 15; // Offset Skalierung wenn Hook geworfen wird
	const float movementVectorScale = -25; // 
	const float maxDistanceValue = 2200;

	const float lookAtOffset = 200;
}

namespace rope{
	const Vec3f translationVector = Vec3f(0,0,0);
	const Vec3f scaleVector = Vec3f(1,1,0.5) * 8;
	const Vec3f gravityVector = Vec3f(0,-0.2,0); // TODO: konfiguration
	const int pieces = 80;
	const long maxRopeLength = 30000; // ohne scale
}

namespace physics{
	// Problem: Werte m�ssen / sollten mit allgemeinem Skalierungsfaktor verrechnet werden
	const float speed = 0.004; // TODO: Geschwindigkeit richtig einstellen
	const int heightDimension = 1; // 0 = x-axis, 1 = y-axis, 2 = z-axis
	const double floorValue = -100000.0; // TODO: tiefsten Punkt einstellen
	const float speedloss = 0.001; // TODO: Geschwindigkeit verlust richtig einstellen
	const float gravity = 10; // TODO: Schwerkraft simulieren
	const float minDirectionLengthValue = 10; // TODO, ist nicht der richtige Wert !
}


// TODO:
// Idee: dynamische hit Distanz, je nach Plattform, als 4D Vektor
namespace pltPositions{
	const int size = 12;
	
	const Vec3f offset = Vec3f(0,100,0);
	const Vec3f positions[12] = { 
		Vec3f(30,-80,30),  // 0, -134, 0
		Vec3f(-1204,-415,-563),
		Vec3f(-733, -640, -2500),
		Vec3f(-2611, -702, -2525),
		Vec3f(-4200, -516, -2383),  
		Vec3f(-4566, -666, -4122), 
		Vec3f(-4343, -810, -5500), 
		Vec3f(-7277, -1290, -7274), // TODO 
		Vec3f(-6935, -2056, -9852),
		Vec3f(-9397, -627, -9005),
		Vec3f(-9236, -1633, -11737),
		Vec3f(-12619, -2718, -11006)
	};
	const double pi2 = 2 * atan(1)*4;
	const float rotation[12] = {
		1.0f,
		pi2 - 0.3f,
		2.1f,
		1.3f,
		2.9f,
		3.1f,
		1.0f, // TODO
		1.0f, // TODO
		1.0f, // TODO
		1.0f // TODO
	};

	const Vec3f ropeOrigins[12] = {
		Vec3f(0,-80, 0),  // 0, -134, 0
		Vec3f(-1204,-417,-563),
		Vec3f(-733, -638, -2500),
		Vec3f(-2611, -700, -2525),
		Vec3f(-4200, -533, -2383),  
		Vec3f(-4566, -664, -4122), 
		Vec3f(-4343, -810, -5500), 
		Vec3f(-7277, -1284, -7274), // TODO 
		Vec3f(-6935, -2056, -9852),
		Vec3f(-9397, -627, -9005),
		Vec3f(-9236, -1633, -11737),
		Vec3f(-12619, -2718, -11006)
	};

	const float maxDistance = 50;
	// int lotto[MAXLOTTO] = { 12, 7, 45, 2, 21, 9 };

}
