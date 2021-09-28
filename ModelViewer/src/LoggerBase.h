/**
 * Base class for the logging system
 *
 * @class LoggerBase
 *
 *
 * @file src/LoggerBase.h
 * @author Olivier Falconnet
 * @date 20200521 - File creation
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <string>
#include "onullstream.h"
//#include <boost/iostreams/stream.hpp>


namespace nxn {


enum SeverityLevel
{
	// Logger severity levels range from 0 to 4
	SeverityDebug = 0,      // Debug severity level
	SeverityInfo,           // Information severity level
	SeverityWarning,        // Warning severity level
	SeverityError,          // Error severity level
	SeverityFatal,          // Fatal severity level
	SeverityNoLog           // No logs
};


class LoggerBase {

public:
	                                            ~LoggerBase();

	static          const   LoggerBase*         getLogger   ();
	        inline  const   bool                isActive    () const                    { return m_active; }
            inline          void                setActive   (bool p_active)             { m_active = p_active; }

	        inline  const   SeverityLevel       getLogLevel () const                    { return m_logLevel; }
            inline          void                setLogLevel (SeverityLevel p_logLevel)  { m_logLevel = p_logLevel; }

	/**
	 * Logs the provided message with a timestamp and File:Line tag in front
	 *
	 * @param {std::string}     p_message       The message to log
	 * @param {SeverityLevel}   p_severityLevel The severity level for the message to log
	 * @param {std::string}     p_sourceFile    The source file where the logger has been called from
	 * @param {int}             p_sourceLine    The line number where the logger has been called from
	 */
    virtual                 void                logMessage  (   const std::string &p_message)               const = 0;
	virtual                 void                logMessage  (   const std::string &p_message,
                                                                SeverityLevel p_severityLevel,
		                                                        std::string p_sourceFile, int p_sourceLine) const = 0;

    /**
     * Returns a reference to the stream and inserts a timestamp and File:Line tag inside
     *
     * @param {SeverityLevel}   p_severityLevel The severity level for the message to log
     * @param {std::string}     p_sourceFile    The source file where the logger has been called from
     * @param {int}             p_sourceLine    The line number where the logger has been called from
     *
     * @return {std::ostream&} A reference to the ostream where the logger sends the messages that are given to him
     */
    virtual                 std::ostream&       getStream   ()                                              const = 0;
    virtual                 std::ostream&       getStream   (   SeverityLevel p_severityLevel,
                                                                std::string p_sourceFile, int p_sourceLine) const = 0;

    static                  std::onullstream    g_nullStream;
    //static                boost::iostreams::stream< boost::iostreams::null_sink > g_nullStream;

protected:
	                                            LoggerBase  (   bool p_makeDefault, SeverityLevel p_logLevel = SeverityInfo);  // Ctor is protected because we only allow derived classes to instantiate this interface

	        inline  const   bool                shouldLog   (   SeverityLevel p_severityLevel) const    { return p_severityLevel != SeverityNoLog && p_severityLevel >= m_logLevel; }
	static                  void                writeTag    (   std::ostream &p_ostream, 
                                                                SeverityLevel p_severityLevel,
		                                                        std::string p_sourceFile, int p_sourceLine);

private:
                                                LoggerBase  (const LoggerBase&);    // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            LoggerBase&         operator=   (const LoggerBase&);    // Intentionally undefined. Is private because we do not allow copies of a Singleton.

	static                  LoggerBase*         g_defaultInstance;  // A pointer to the default logger. This is not a singleton pointer.
	                        bool                m_active;
	                        SeverityLevel       m_logLevel;         // The log level allows to filter messages, and only log those above the given severity threshold

}; // class LoggerBase

} // namespace nxn


#include <filesystem>

#define LogMessage(level, msg)      nxn::LoggerBase::getLogger()->logMessage    (msg,   level, std::filesystem::path(__FILE__).filename().string(), __LINE__);
#define GetLogStream(level)	        nxn::LoggerBase::getLogger()->getStream     (       level, std::filesystem::path(__FILE__).filename().string(), __LINE__)

//#define LogMessage(level, msg)
//#define GetLogStream(level)       nxn::LoggerBase::g_nullStream
