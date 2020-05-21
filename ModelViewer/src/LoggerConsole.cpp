#include "LoggerConsole.h"

#include <iostream>


enum ConsoleTextColor {
	Default = 39,
	black = 30,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	lightGray,
	darkGray = 90,
	lightRed,
	lightGreen,
	lightYellow,
	lightBlue,
	lightMagenta,
	lightCyan,
	white
};

void setSeverityColor(SeverityLevel pSeverityLevel)
{
	switch (pSeverityLevel) {

	case SeverityDebug:
		std::cout << "\033[" << ConsoleTextColor::lightBlue << "m";
		break;
	case SeverityInfo:
		std::cout << "\033[" << ConsoleTextColor::lightGreen << "m";
		break;
	case SeverityWarning:
		std::cout << "\033[" << ConsoleTextColor::yellow << "m";
		break;
	case SeverityError:
		std::cout << "\033[" << ConsoleTextColor::lightRed << "m";
		break;
	case SeverityFatal:
		std::cout << "\033[" << ConsoleTextColor::lightRed << "m";
		break;
	default:
	case SeverityNoLog:
		break;
	}
}

void resetSeverityColor()
{
	std::cout << "\033[" << ConsoleTextColor::Default << "m";
}


LoggerConsole::LoggerConsole(bool pMakeDefault, SeverityLevel pLogLevel) :
	LoggerBase(pMakeDefault, pLogLevel)
{
	logMessage("ConsoleLogger::ctor()", SeverityInfo, "ConsoleLogger.cpp", __LINE__);
}

LoggerConsole::~LoggerConsole()
{
	logMessage("ConsoleLogger::dtor()", SeverityInfo, "ConsoleLogger.cpp", __LINE__);
	resetSeverityColor();
}


std::ostream &LoggerConsole::getStream()
{
	return std::cout;
}

std::ostream &LoggerConsole::getStream(SeverityLevel pSeverityLevel, std::string pSourceFile, int pSourceLine)
{
	setSeverityColor(pSeverityLevel);
	writeTag(std::cout, pSeverityLevel, pSourceFile, pSourceLine);
	resetSeverityColor();

	return std::cout;
}

void LoggerConsole::logMessage(const std::string &pMessage)
{
	if (isActive()) 
	{
		std::cout << pMessage << std::endl;
	}
}

void LoggerConsole::logMessage(const std::string &pMessage, SeverityLevel pSeverityLevel, std::string pSourceFile,
	int pSourceLine)
{
	if (isActive() && shouldLog(pSeverityLevel)) 
	{
		setSeverityColor(pSeverityLevel);
		writeTag(std::cout, pSeverityLevel, pSourceFile, pSourceLine);
		resetSeverityColor();

		std::cout << pMessage << std::endl;
	}
}