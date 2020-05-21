/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include <windows.h>
#include <iostream>

#include "Timer.h"
#include "LoggerConsole.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{
	LoggerConsole logger(true, SeverityDebug);

	Timer timer;
	timer.init();

	Sleep(20);

	// We should have a little bit (< 1 ms) more than the sleep duration
	float elaspedTime = timer.getElapsedTime();
	GetLogStream(SeverityDebug) << "elapsedTime : " << std::to_string(elaspedTime) << std::endl;
}


/*
int main()
{
	//std::cout << "Hello world !" << std::endl;

	LoggerConsole logger(true, SeverityDebug);

	LogMessage(SeverityDebug,	"Test Debug");
	//logger.setLogLevel(SeverityWarning);
	LogMessage(SeverityInfo,	"Test Info");
	LogMessage(SeverityWarning, "Test Warning");
	LogMessage(SeverityError,	"Test Error");
	LogMessage(SeverityFatal,	"Test Fatal");

	return 0;
}
//*/

/*
int main()
{
	std::cout << "Hello world !" << std::endl;
}
//*/