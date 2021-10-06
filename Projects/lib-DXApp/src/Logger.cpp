#include "pch.h"
#include "Logger.h"


namespace nxn {


std::onullstream Logger::g_nullStream;
//boost::iostreams::stream< boost::iostreams::null_sink > Logger::g_nullStream( ( boost::iostreams::null_sink() ) );
Logger* Logger::g_defaultInstance = NULL;

//-----------------------------------------------------------------------------
/**
  * @param {bool}           p_makeDefault   true if the new logger must become the default logger, false if not
  * @param {SeverityLevel}  p_logLevel      The log level for the logger
  */
Logger::Logger(bool p_makeDefault, SeverityLevel p_logLevel) :
	m_active(false)
{
	if (p_makeDefault) {
		g_defaultInstance = this;
	}

	SetActive(true);
	SetLogLevel(p_logLevel);
}

//-----------------------------------------------------------------------------

Logger::~Logger()
{
	SetLogLevel(SeverityNoLog);
	SetActive(false);

	if (this == g_defaultInstance) {
		g_defaultInstance = NULL;
	}
}

//-----------------------------------------------------------------------------

const Logger* Logger::GetLogger()
{
	// If there is no logger instantiated, instantiate one at the last moment
	if (NULL == g_defaultInstance) {
		WriteLogMessage(SeverityFatal, "LoggerBase::getLogger() : No logger has been instantiated")
		throw new std::exception("No logger has been instantiated");
	}

	return g_defaultInstance;
}

//-----------------------------------------------------------------------------
/**
 * Write a timestamp and File:Line tag to the provided ostream
 * @param {std::ostream} p_ostream The ostream where to write the prefix tag
 * @param {SeverityLevel} p_severityLevel The severity level for the message to log
 * @param {std::string} p_sourceFile The source file where the logger has been called from
 * @param {int} p_sourceLine The line number where the logger has been called from
 */
void Logger::WriteTag(  std::ostream &p_ostream, 
                        SeverityLevel p_severityLevel, 
                        std::string p_sourceFile, int p_sourceLine)
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

	// Write the log level as a single character
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

} // namespace nxn
