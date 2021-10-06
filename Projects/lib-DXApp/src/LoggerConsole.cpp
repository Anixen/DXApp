#include "pch.h"
#include "LoggerConsole.h"


namespace nxn {


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

//-----------------------------------------------------------------------------

void SetSeverityColor(SeverityLevel p_severityLevel)
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

void ResetSeverityColor()
{
	std::cout << "\033[" << ConsoleTextColor::Default << "m";
}

//-----------------------------------------------------------------------------
/**
 * LoggerConsole constructor
 *
 * @param {bool} p_makeDefault True if the new logger must become the default logger, false if not
 * @param {SeverityLevel} p_logLevel The log level for the logger
 */
LoggerConsole::LoggerConsole(bool p_makeDefault, SeverityLevel p_logLevel) :
	Logger(p_makeDefault, p_logLevel)
{
	WriteLogMessage(SeverityInfo, "ConsoleLogger::ctor()")
}

//-----------------------------------------------------------------------------
/**
 * LoggerConsole deconstructor
 */
LoggerConsole::~LoggerConsole()
{
	WriteLogMessage(SeverityInfo, "ConsoleLogger::dtor()")
	ResetSeverityColor();
}

//-----------------------------------------------------------------------------
/**
 * @return {std::ostream&} a reference to the ostream where the logger sends the messages that are given to him
 */
std::ostream &LoggerConsole::GetStream() const
{
	return std::cout;
}

//-----------------------------------------------------------------------------
/**
 * Returns a reference to the stream and inserts a timestamp and File:Line tag inside
 *
 * @return {std::ostream&} a reference to the ostream where the logger sends the messages that are given to him
 */
std::ostream &LoggerConsole::GetStream( SeverityLevel p_severityLevel, 
                                        std::string p_sourceFile, int p_sourceLine) const
{
	SetSeverityColor(p_severityLevel);
	WriteTag(std::cout, p_severityLevel, p_sourceFile, p_sourceLine);
	ResetSeverityColor();

	return std::cout;
}

//-----------------------------------------------------------------------------
/**
 * Logs the provided message
 *
 * @param {std::string} p_message The message to log
 */
void LoggerConsole::WriteMessage(const std::string &p_message) const
{
	if (IsActive()) 
	{
		std::cout << p_message << std::endl;
	}
}

//-----------------------------------------------------------------------------
/**
 * Logs the provided message with a timestamp and File:Line tag in front
 *
 * @param {std::string} p_message The message to log
 * @param {SeverityLevel} p_severityLevel The severity level for the message to log
 * @param {std::string} p_sourceFile The source file where the logger has been called from
 * @param {int} p_sourceLine The line number where the logger has been called from
 */
void LoggerConsole::WriteMessage(const std::string &p_message, 
                                    SeverityLevel p_severityLevel,
                                    std::string p_sourceFile, int p_sourceLine) const
{
	if (IsActive() && ShouldLog(p_severityLevel)) 
	{
		SetSeverityColor(p_severityLevel);
		WriteTag(std::cout, p_severityLevel, p_sourceFile, p_sourceLine);
		ResetSeverityColor();

		std::cout << p_message << std::endl;
	}
}

} // namespace nxn
