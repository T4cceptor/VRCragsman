#include "OSGVector.h"
#include <string.h>
#include "OSGColor.h"

namespace config{
	const int test = 0;
}

namespace path{
	const std::string caveModel = "../models/Cave10.obj";
	const std::string hookModel = "../models/hookwithknot3.obj";
	const std::string ropeModel = "../models/ropeonly.obj";
	const std::string anchorModel = "../models/Anker.obj";
	const std::string icicleModel = "../models/BasicIcicle1Only.obj";
	const std::string stalactitesModel = "../models/stalactites3.obj";
	const std::string pebbleModel = "../models/pebble2.obj";
	const std::string pebbleModel2 = "../models/pebble3.obj";
}

namespace general {
	const double scale = 5.5; // muss entsprechend der Ausfuehrungsumgebung angepasst werden!
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
	const Vec3f translationVector = Vec3f(0,0,0); // Anf�nglicher Translations Vektor
	const int caveScale = 40;
	const Vec3f scaleVector = Vec3f(1,1,1) * caveScale; // Skalierung des 3D Models
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
	const float movementVectorScale = -25;
	const float maxDistanceValue = 2200;

	const float lookAtOffset = 200;
}

namespace rope{
	const Vec3f translationVector = Vec3f(0,0,0);
	const Vec3f scaleVector = Vec3f(1,1,0.5) * 8;
	const Vec3f gravityVector = Vec3f(0,-0.2,0);
	const int pieces = 80;
	const long maxRopeLength = 30000; // ohne scale
}

// TODO
namespace icicles{
	const Vec3f scaleVector = Vec3f(1, 1, 1) * 20;
	const int positionsSize = 5;
	const Vec3f positions[positionsSize] = { 
		Vec3f(-10,-100, -30),
		Vec3f( -1750, -420, -620),
		Vec3f(-1040,-80,-540),
		Vec3f(-1540,-2500,-740),
		Vec3f(-2040,-2500,-940),
	};
}

namespace stalactites{
	const int scale = 40;
	const Vec3f scaleVector = Vec3f(1, 1, 1) * scale;
	const int positionsSize = 22;
	const int offsetScale = 40;
	const Vec3f positions[positionsSize] = { 
		Vec3f(-12, 31, -6) * offsetScale ,
		Vec3f(-18, 28, -5) * offsetScale,
		Vec3f(-4, 31, -13) * offsetScale,
		Vec3f(-9, 28, -17) * offsetScale,
		Vec3f(-20, 23, -19) * offsetScale,
		Vec3f(-20, 24, -28) * offsetScale,
		Vec3f(-21, 26, -11) * offsetScale,
		Vec3f(-30.92612, 15.91989, -46.76899) * offsetScale,
		Vec3f(-30.00424, 21.03165, -20.66109) * offsetScale,
		Vec3f(-22.85489, 22.20384, -37.40468) * offsetScale,
		Vec3f(-33.53892, 17.63613, -27.31742) * offsetScale,
		Vec3f(-24.67679, 21.28176, -38.46682) * offsetScale,
		Vec3f(-33.11415, 18.5697, -21.09117) * offsetScale,
		Vec3f(-28.27766, 18.37574, -24.48655) * offsetScale,
		Vec3f(-24.0337, 17.21975, -54.02957) * offsetScale,
		Vec3f(-60.0996, 28.96903, -44.41283) * offsetScale,
		Vec3f(-85.53035, 26.23298, -57.87487) * offsetScale,
		Vec3f(-151.34204, 21.16491, -131.65057) * offsetScale,
		Vec3f(-142.69237, 12.07856, -143.71127) * offsetScale,
		Vec3f(-175.76003, 50.14698, -222.00285) * offsetScale,
		Vec3f(-243.95926, 27.56112, -262.1427) * offsetScale,
		Vec3f(-226.93118, 25.50752, -273.50171) * offsetScale
	};
}

namespace pebble{
	const int scale = 40;
	const Vec3f scaleVector = Vec3f(1, 1, 1) * scale;
	const int offsetScale = 40;

	const int positionsSize = 17;
	const Vec3f positions[positionsSize] = { 
		Vec3f(-4.1972, -2.12707, -26.92875) * offsetScale ,
		Vec3f(-8.82521, 1.95082, -63.77443) * offsetScale ,
		Vec3f(-10.83841, -9.48832, -45.31162) * offsetScale,
		Vec3f(-26.34115, -16.18159, -7.26035) * offsetScale ,
		Vec3f(-34.73042, -17.19321, -22.55859) * offsetScale ,
		Vec3f(-36.22649, -9.55864, -34.46089) * offsetScale ,
		Vec3f(-42.77905, 3.07479, -16.05574) * offsetScale ,
		Vec3f(-75.74564, 16.32944, -36.55459) * offsetScale,
		Vec3f(-81.94243, 1.59627, -86.01863) * offsetScale,
		Vec3f(-93.67945, -13.14307, -116.6429) * offsetScale,
		Vec3f(-103.52679, -21.66015, -56.07335) * offsetScale,
		Vec3f(-112.19285, 9.05937, -79.82671) * offsetScale,
		Vec3f(-115.4119, -22.89444, -96.5978) * offsetScale,
		Vec3f(-117.42213, -17.14842, -152.59164) * offsetScale,
		Vec3f(-127.93298, -7.69315, -100.61451) * offsetScale,
		Vec3f(-144.0442, 4.76495, -155.38074) * offsetScale,
		Vec3f(-180.99635, 6.3054, 133.76112) * offsetScale
	};

	const int positionsSize2 = 11;
	const Vec3f positions2[positionsSize] = { 
		Vec3f(-12.81623, 8.4126, -36.46515) * offsetScale,
		Vec3f(-50.3685, 4.85967, -70.03362) * offsetScale,
		Vec3f(-71.87481, 3.69589, -38.94099) * offsetScale,
		Vec3f(-92.677, -25.76001, -117.53835) * offsetScale,
		Vec3f(-109.43048, 13.49724, -81.89369) * offsetScale,
		Vec3f(-114.08493, -3.39178, -158.00041) * offsetScale,
		Vec3f(-114.54091, -41.0415, -153.40923) * offsetScale,
		Vec3f(-137.16963, 16.54248, -139.55417) * offsetScale,
		Vec3f(-160.75143, 8.14473, -157.81619) * offsetScale,
		Vec3f(-161.97505, -32.27337, -116.63432) * offsetScale,
		Vec3f(-194.98488, -8.22668, -157.26208) * offsetScale
	};
}

namespace specialObjs{
	const int count = 3;
	const int scale = 40;
	const Vec3f scaleVector = Vec3f(1, 1, 1) * scale;
	const std::string paths[count] = {
		"../models/stalactites2.obj",
		"../models/pebble1.obj",
		"../models/meshes2.obj"
	};
}

namespace configLight{
	const Color4f globalDiffuse = Color4f(0.6,0.6,0.6,1);
	const Color4f globalAmbient = Color4f(0.45f,0.45f,0.45f,1);
	const Color4f globalSpecular = Color4f(0.3,0.3,0.3,1);
	const int positionsSize = 2;
	const Vec3f positions[positionsSize] = { 
		Vec3f( 9.68455, 37, 7.45946),
		Vec3f(-20000,-3000,-15000)
	};
	const Vec3f attenuations[positionsSize] = {
		Vec3f (0, 0.0002, 0),
		Vec3f (0, 0.00001, 0)
		//Vec3f (5,20.01,20),
		//Vec3f (5,20.065,20),
		//Vec3f (5,20.075,20)
		//Vec3f (2,0.00002,0),
		//Vec3f (3,0.0001,0),
		//Vec3f (3,0.0001,0)
	};
	const Vec4f diffuseColors[positionsSize] = {
		Vec4f (0.465,0.468,0.5,0.5), 
		Vec4f (0.56,0.45,0.5,0.7)
		//Vec4f (0.2,0.4,0.4,0.5),
		//Vec4f (0.4,0.4,0.4,0.5),
		//Vec4f (0.4,0.3,0.4,0.5)
	};
	const Vec4f ambientColors[positionsSize] = {
		Vec4f (0.15f,0.15f,0.15f,0.2), // R: 255, G: 255, B: 204
		Vec4f (0.11f,0.11f,0.11f,0.7)
		//Vec4f (0.2f,0.3f,0.3f,0.5),
		//Vec4f (0.3f,0.3f,0.4f,0.5),
		//Vec4f (0.5f,0.3f,0.3f,0.5)
	};
	const Vec4f specularColors[positionsSize] = {
		Vec4f (0.6,0.6,0.6,0.5), // R: 255, G: 153, B: 51
		Vec4f (0.3,0.3,0.5,0.7)
		//Vec4f (0.3,0.3,0.3,0.5),
		//Vec4f (0.3,0.3,0.3,0.5),
		//Vec4f (0.3,0.3,0.3,0.5)
	};
}

namespace platforms{
	const int scale = 40;
	const int count = 12;
	const Vec3f scaleVector = Vec3f(1,1,1) * scale;
	const Vec3f offset = Vec3f(0,45,0);
	const Vec3f constOffset = Vec3f(0,10,0);
	const Vec3f positions[count] = { 
		Vec3f(0,0,0),  // 0, -134, 0
		Vec3f(-30.08339, -8.88202, -17.80571) * scale + constOffset,
		Vec3f(-22.35297, -14.32559, -62.10383) * scale + constOffset,
		Vec3f(-70.36655, -15.85978, -65.15252) * scale + constOffset,
		Vec3f(-103.70795, -11.27097, -64.18584) * scale + constOffset,
		Vec3f(-116.57938, -14.09145, -106.00539) * scale + constOffset,
		Vec3f(-110.75109, -17.77441, -140.15552) * scale + constOffset,
		Vec3f(-171.05093, -16.94497, -129.41019) * scale + constOffset,
		Vec3f(-182.94658, -31.4542, -174.64622) * scale + constOffset,
		Vec3f(-182.10548, -49.48945, -243.90367) * scale + constOffset,
		Vec3f(-228.05298, -14.79423, -241.67993) * scale + constOffset, // -228.05298, -14.79423, -241.67993
		Vec3f(-234.72742, -35.22293, -290.99417) * scale + constOffset
	};

	const std::string paths[count] = {
		"../models/pltform0.obj",
		"../models/pltform1.obj",
		"../models/pltform2.obj",
		"../models/pltform3.obj",
		"../models/pltform4.obj",
		"../models/pltform5.obj",
		"../models/pltform6.obj",
		"../models/pltform7.obj",
		"../models/pltform8.obj",
		"../models/pltform9.obj",
		"../models/pltform10.obj",
		"../models/pltform11.obj"
	};
	const Vec3f ropeOrigins[count] = {
		Vec3f(0,0,0),  // 0, -134, 0
		Vec3f(-30.08339, -8.88202, -17.80571) * scale + constOffset,
		Vec3f(-22.35297, -14.32559, -62.10383) * scale + constOffset,
		Vec3f(-70.36655, -15.85978, -65.15252) * scale + constOffset,
		Vec3f(-103.70795, -11.27097, -64.18584) * scale + constOffset,
		Vec3f(-116.57938, -14.09145, -106.00539) * scale + constOffset,
		Vec3f(-110.75109, -17.77441, -140.15552) * scale + constOffset,
		Vec3f(-171.05093, -16.94497, -129.41019) * scale + constOffset,
		Vec3f(-182.94658, -31.4542, -174.64622) * scale + constOffset,
		Vec3f(-182.10548, -49.48945, -243.90367) * scale + constOffset,
		Vec3f(-228.05298, -14.79423, -241.67993) * scale + constOffset,
		Vec3f(-234.72742, -35.22293, -290.99417) * scale + constOffset
	};
	const float maxDistance = 50;
}

namespace physics{
	const float speed = 0.004; // TODO: Geschwindigkeit richtig einstellen
	const int heightDimension = 1; // 0 = x-axis, 1 = y-axis, 2 = z-axis
	const double floorValue = -100000.0; // TODO: tiefsten Punkt einstellen
	const float speedloss = 0.001; // TODO: Geschwindigkeit verlust richtig einstellen
	const float gravity = 10; // TODO: Schwerkraft simulieren
	const float minDirectionLengthValue = 10; // TODO, ist nicht der richtige Wert !
}