#include "OSGVector.h"
#include <string.h>
#include "OSGColor.h"

namespace config{
	const int test = 0;
}

namespace path{
	const std::string caveModel = "../models/Cave2.obj";
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


	const Vec3f positions[1] = { 
		Vec3f(540,50,540)
	};

}

namespace general {
	const double scale = 6.0; // muss entsprechend der Ausf�hrungsumgebung angepasst werden!
	const double movementFactor = 10.0;

	const Vec3f upVector = Vec3f(0,1,0); // je nach Koordinatensystem einstellen
	const Vec3f rightVector = Vec3f(1,0,0); // je nach Koordinatensystem einstellen
	const Vec3f forwardVector = Vec3f(0,0,-1); // je nach Koordinatensystem einstellen

	const float plattformHitDistance = 300.0f;
	const float minDistanceToFloor = 50.0f;
	const float hitDistance = 20.0f;
	const int ticksPerSecond = 50;

	const int maxCount = 100;
}

namespace cave{
	const Vec3f translationVector = Vec3f(370,0,330); // Anf�nglicher Translations Vektor
	const Vec3f scaleVector = Vec3f(10,10,10); // Skalierung des 3D Models
	const float velocityReduction = 0.2; // gibt Prozentwert an, auf den die Geschwindigkeit eines auftreffenden Objekts veringert wird
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
	const float maxDistanceValue = 2000;

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
	const float minDirectionLengthValue = 2; // TODO, ist nicht der richtige Wert !
}

// TODO:
// Idee: dynamische hit Distanz, je nach Plattform, als 4D Vektor
namespace pltPositions{
	const int size = 12;
	const Vec3f positions[12] = { 
		Vec3f(0,-67,0),  // 0, -134, 0
		Vec3f(-1204,-411,-563),
		Vec3f(-646, -579, -2355),
		Vec3f(-2885, -460, -2480),
		Vec3f(-4464, -535, -3730),  
		Vec3f(-4139, -823, -5308), 
		Vec3f(-6509, -834, -4719), 
		Vec3f(-7277, -1324, -7274), 
		Vec3f(-6935, -2056, -9852),
		Vec3f(-9397, -627, -9005),
		Vec3f(-9236, -1633, -11737),
		Vec3f(-12619, -2718, -11006)
	};
	const float rotation[12] = {
		1.0f,
		0.5f,
		1.4f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f
	};

	const Vec3f ropeOrigins[12] = {
		Vec3f(0,-67,0),  // 0, -134, 0
		Vec3f(-1204,-411,-563),
		Vec3f(-646, -579, -2355),
		Vec3f(-2885, -460, -2480),
		Vec3f(-4464, -535, -3730),  
		Vec3f(-4139, -823, -5308), 
		Vec3f(-6509, -834, -4719), 
		Vec3f(-7277, -1324, -7274), 
		Vec3f(-6935, -2056, -9852),
		Vec3f(-9397, -627, -9005),
		Vec3f(-9236, -1633, -11737),
		Vec3f(-12619, -2718, -11006)
	};

	const float maxDistance = 50;
	// int lotto[MAXLOTTO] = { 12, 7, 45, 2, 21, 9 };

}
