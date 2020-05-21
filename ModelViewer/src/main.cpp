/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include <windows.h>
#include <iostream>

#include "LoggerConsole.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{
	//std::cout << "Hello world !" << std::endl;

	LoggerConsole logger(true, SeverityDebug);

	LogMessage("Test Debug", SeverityDebug);
	//logger.setLogLevel(SeverityWarning);
	LogMessage("Test Info", SeverityInfo);
	LogMessage("Test Warning", SeverityWarning);
	LogMessage("Test Error", SeverityError);
	LogMessage("Test Fatal", SeverityFatal);

	return 0;
}