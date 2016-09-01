#include "VRGLogger.h"


/*
VRGLogger::VRGLogger(void)
{
}

VRGLogger::~VRGLogger(void)
{
}
*/

std::string logFileName = "log.txt";

bool exists_test3 (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool logFileExists(){
	return exists_test3(logFileName);
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void VRGLogger::logMessage(std::string message){

	if(logFileExists())
	{
		//std::ofstream outfile ("test.txt");
		std::ofstream outfile;
		outfile.open(logFileName, std::ios::out | std::ios::app);
		outfile << message.c_str() << " at: " << currentDateTime() << std::endl;
		std::cout << message.c_str() << " at: " << currentDateTime() << std::endl;
		outfile.close();
	}
	else {
		std::ofstream myfile;
		myfile.open (logFileName);
		myfile << message.c_str() << " at: " << currentDateTime() << std::endl;
		std::cout << message.c_str() << " at: " << currentDateTime() << std::endl;
		myfile.close();
	}
}