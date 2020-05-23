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

void setSeverityColor(SeverityLevel p_severityLevel)
{
	switch (p_severityLevel) 
	{
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


LoggerConsole::LoggerConsole(bool p_makeDefault, SeverityLevel p_logLevel) :
	LoggerBase(p_makeDefault, p_logLevel)
{
	LogMessage(SeverityInfo, "ConsoleLogger::ctor()")
}

LoggerConsole::~LoggerConsole()
{
	LogMessage(SeverityInfo, "ConsoleLogger::dtor()")
	resetSeverityColor();
}


std::ostream &LoggerConsole::getStream()
{
	return std::cout;
}

std::ostream &LoggerConsole::getStream(SeverityLevel p_severityLevel, std::string p_sourceFile, int p_sourceLine)
{
	setSeverityColor(p_severityLevel);
	writeTag(std::cout, p_severityLevel, p_sourceFile, p_sourceLine);
	resetSeverityColor();

	return std::cout;
}

void LoggerConsole::logMessage(const std::string &p_message)
{
	if (isActive()) 
	{
		std::cout << p_message << std::endl;
	}
}

void LoggerConsole::logMessage(const std::string &p_message, SeverityLevel p_severityLevel, std::string p_sourceFile,
	int p_sourceLine)
{
	if (isActive() && shouldLog(p_severityLevel)) 
	{
		setSeverityColor(p_severityLevel);
		writeTag(std::cout, p_severityLevel, p_sourceFile, p_sourceLine);
		resetSeverityColor();

		std::cout << p_message << std::endl;
	}
}