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

	LogMessage(SeverityDebug,	"Test Debug");
	//logger.setLogLevel(SeverityWarning);
	LogMessage(SeverityInfo,	"Test Info");
	LogMessage(SeverityWarning, "Test Warning");
	LogMessage(SeverityError,	"Test Error");
	LogMessage(SeverityFatal,	"Test Fatal");

	return 0;
}