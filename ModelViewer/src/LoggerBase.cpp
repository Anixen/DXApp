#include "LoggerBase.h"

#include <time.h>
#include <sstream>


LoggerBase* LoggerBase::gInstance = NULL;

// TODO : Implement onnullstream
//ILogger::_nullStream;

LoggerBase::LoggerBase(bool pMakeDefault, SeverityLevel pLogLevel) :
	mActive(false)
{
	if (pMakeDefault) {
		gInstance = this;
	}

	setActive(true);
	setLogLevel(pLogLevel);
}

LoggerBase::~LoggerBase()
{
	setLogLevel(SeverityNoLog);
	setActive(false);

	if (this == gInstance) {
		gInstance = NULL;
	}
}

LoggerBase* LoggerBase::getLogger()
{
	// If there is no logger instantiated, instantiate one at the last moment
	if (NULL == gInstance) {
		LogMessage(SeverityFatal, "LoggerBase::getLogger() : No logger has been instantiated")
	}

	return gInstance;
}

bool LoggerBase::isActive()
{
	return mActive;
}

void LoggerBase::setActive(bool pActive)
{
	mActive = pActive;
}

SeverityLevel LoggerBase::getLogLevel()
{
	return mLogLevel;
}

void LoggerBase::setLogLevel(SeverityLevel pLogLevel)
{
	mLogLevel = pLogLevel;
}


bool LoggerBase::shouldLog(SeverityLevel pSeverityLevel) 
{
	return pSeverityLevel != SeverityNoLog
		&& pSeverityLevel >= mLogLevel;
}

void LoggerBase::writeTag(std::ostream &pOstream, SeverityLevel pSeverityLevel, std::string pSourceFile,
	int pSourceLine)
{
	// Get local time
	std::tm localTime;
	{
		// Get current time (seconds since January 1st 1970)
		__time64_t now;
		_time64(&now);

		// Convert current time into local time
		errno_t err = _localtime64_s(&localTime, &now);
		if (err)
		{
			// Do not use LogMessage() here
			// to avoid a potential recursive scenario if _localtime64_s() doesnt work 
			throw new std::exception("Invalid argument to _localtime64_s");
		}
	}

	// Construct timestamp
	std::ostringstream timestamp, tag;
	{
		// Year
		timestamp << localTime.tm_year + 1900 << "-";
		// Month
		timestamp.fill('0');
		timestamp.width(2);
		timestamp << localTime.tm_mon + 1 << "-";
		// Day
		timestamp.fill('0');
		timestamp.width(2);
		timestamp << localTime.tm_mday << " ";

		// Hour
		timestamp.fill('0');
		timestamp.width(2);
		timestamp << localTime.tm_hour << ":";
		// Minutes
		timestamp.fill('0');
		timestamp.width(2);
		timestamp << localTime.tm_min << ":";
		// Seconds
		timestamp.fill('0');
		timestamp.width(2);
		timestamp << localTime.tm_sec << " ";
	}

	// Cast the log level as a single character
	char severityLevel;
	{
		switch (pSeverityLevel)
		{
		case SeverityDebug:
			severityLevel = 'D';
			break;
		case SeverityInfo:
			severityLevel = 'I';
			break;
		case SeverityWarning:
			severityLevel = 'W';
			break;
		case SeverityError:
			severityLevel = 'E';
			break;
		case SeverityFatal:
			severityLevel = 'F';
			break;
		default:
		case SeverityNoLog:
			severityLevel = '-';
			break;
		}
	}
	
	// Construct the File:Line tag
	tag << pSourceFile << ":" << pSourceLine;

	// Output the timestamp and File:Line tag into the ostream
	pOstream << timestamp.str() << " " << severityLevel << " " << tag.str() << " ";
}
