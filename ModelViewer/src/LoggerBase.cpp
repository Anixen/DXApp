#include "LoggerBase.h"

#include <time.h>
#include <sstream>


std::onullstream LoggerBase::g_nullStream;
//boost::iostreams::stream< boost::iostreams::null_sink > LoggerBase::g_nullStream( ( boost::iostreams::null_sink() ) );
LoggerBase* LoggerBase::g_instance = NULL;

LoggerBase::LoggerBase(bool p_makeDefault, SeverityLevel p_logLevel) :
	m_active(false)
{
	if (p_makeDefault) {
		g_instance = this;
	}

	setActive(true);
	setLogLevel(p_logLevel);
}

LoggerBase::~LoggerBase()
{
	setLogLevel(SeverityNoLog);
	setActive(false);

	if (this == g_instance) {
		g_instance = NULL;
	}
}

LoggerBase* LoggerBase::getLogger()
{
	// If there is no logger instantiated, instantiate one at the last moment
	if (NULL == g_instance) {
		LogMessage(SeverityFatal, "LoggerBase::getLogger() : No logger has been instantiated")
		throw new std::exception("No logger has been instantiated");
	}

	return g_instance;
}

bool LoggerBase::isActive()
{
	return m_active;
}

void LoggerBase::setActive(bool p_active)
{
	m_active = p_active;
}

SeverityLevel LoggerBase::getLogLevel()
{
	return m_logLevel;
}

void LoggerBase::setLogLevel(SeverityLevel p_logLevel)
{
	m_logLevel = p_logLevel;
}


bool LoggerBase::shouldLog(SeverityLevel p_severityLevel) 
{
	return p_severityLevel != SeverityNoLog
		&& p_severityLevel >= m_logLevel;
}

void LoggerBase::writeTag(std::ostream &p_ostream, SeverityLevel p_severityLevel, std::string p_sourceFile,
	int p_sourceLine)
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
		switch (p_severityLevel)
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
	tag << p_sourceFile << ":" << p_sourceLine;

	// Output the timestamp and File:Line tag into the ostream
	p_ostream << timestamp.str() << " " << severityLevel << " " << tag.str() << " ";
}
