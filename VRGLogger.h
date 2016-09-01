#pragma once
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <string>
#include <time.h>
#include <stdio.h>

class VRGLogger
{
public:
	// VRGLogger(void);
	// ~VRGLogger(void);
	static void logMessage(std::string message);
};

