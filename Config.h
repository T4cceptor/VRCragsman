#include "OSGVector.h"

namespace config{
	const int test = 0;
}

namespace general {
	const double scale = 2.0; // muss entsprechend der Ausführungsumgebung angepasst werden!
	const double movementFactor = 10.0;

	const Vec3f upVector = Vec3f(0,1,0); // je nach Koordinatensystem einstellen
	const Vec3f rightVector = Vec3f(1,0,0); // je nach Koordinatensystem einstellen
	const Vec3f forwardVector = Vec3f(0,0,-1); // je nach Koordinatensystem einstellen

	const float plattformHitDistance = 250.0f;
	const float minDistanceToFloor = 50.0f;
	const float hitDistance = 20.0f;

}

namespace cave{
	const Vec3f translationVector = Vec3f(370,0,337); // Anfänglicher Translations Vektor
	// alte Werte: Vec3f(5100,300,5100);
	const Vec3f scaleVector = Vec3f(10,10,10); // Skalierung des 3D Models

	const float velocityReduction = 0.2; // gibt Prozentwert an um den die Geschwindigkeit eines auftreffenden Objekts veringert wird

}

namespace hook{
	const Vec3f translationVector = Vec3f(0,0,0); // Anfänglicher Translations Vektor
	const Vec3f scaleVector = Vec3f(1,1,1); // Skalierung des 3D Models
	const float movementOffsetScale = 5; // Offset Skalierung wenn Hook geworfen wird
	const float movementVectorScale = -13; // 
}

namespace physics{
	
	// Problem: Werte müssen / sollten mit allgemeinem Skalierungsfaktor verrechnet werden
	
	const float speed = 0.2; // TODO: Geschwindigkeit richtig einstellen
	const int heightDimension = 1; // 0 = x-axis, 1 = y-axis, 2 = z-axis
	const double floorValue = -100000.0; // TODO: tiefsten Punkt einstellen
	const float speedloss = 0.0001; // TODO: Geschwindigkeit verlust richtig einstellen
	const float gravity = 0.06; // TODO: Schwerkraft simulieren
	const float minDirectionLengthValue = 0.05; // TODO, ist nicht der richtige Wert !

}

// TODO:
// Idee: dynamische hit Distanz, je nach Plattform, als 4D Vektor
namespace pltPositions{
	const int size = 12;
	const Vec3f positions[12] = { 
		Vec3f(0,0,0),  // 0, -134, 0
		Vec3f(-1217,-411,-446), 
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

	const float maxDistance = 50;
	// int lotto[MAXLOTTO] = { 12, 7, 45, 2, 21, 9 };

}
