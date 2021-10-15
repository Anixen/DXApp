/**
 * Base class for the logging system
 *
 * @class Logger
 *
 *
 * @file src/Logger.h
 * @author Olivier Falconnet
 * @date 20200521 - File creation
 */

#ifndef NXN_LOGGER
#define NXN_LOGGER


//#include <string>
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


class Logger {

public:
	                                            ~Logger         ();

	static          const   Logger*             GetLogger       ();
	        inline  const   bool                IsActive        () const                    { return m_active; }
            inline          void                SetActive       (bool p_active)             { m_active = p_active; }

	        inline  const   SeverityLevel       GetLogLevel     () const                    { return m_logLevel; }
            inline          void                SetLogLevel     (SeverityLevel p_logLevel)  { m_logLevel = p_logLevel; }

	/**
	 * Logs the provided message with a timestamp and File:Line tag in front
	 *
	 * @param {std::string}     p_message       The message to log
	 * @param {SeverityLevel}   p_severityLevel The severity level for the message to log
	 * @param {std::string}     p_sourceFile    The source file where the logger has been called from
	 * @param {int}             p_sourceLine    The line number where the logger has been called from
	 */
    virtual                 void                WriteMessage    (   const std::string &p_message)               const = 0;
	virtual                 void                WriteMessage    (   const std::string &p_message,
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
    virtual                 std::ostream&       GetStream       ()                                              const = 0;
    virtual                 std::ostream&       GetStream       (   SeverityLevel p_severityLevel,
                                                                    std::string p_sourceFile, int p_sourceLine) const = 0;

    static                  std::onullstream    g_nullStream;
    //static                boost::iostreams::stream< boost::iostreams::null_sink > g_nullStream;

protected:
	                                            Logger          (   bool p_makeDefault, SeverityLevel p_logLevel = SeverityInfo);  // Ctor is protected because we only allow derived classes to instantiate this interface

	        inline  const   bool                ShouldLog       (   SeverityLevel p_severityLevel) const    { return p_severityLevel != SeverityNoLog && p_severityLevel >= m_logLevel; }
	static                  void                WriteTag        (   std::ostream &p_ostream, 
                                                                    SeverityLevel p_severityLevel,
		                                                            std::string p_sourceFile, int p_sourceLine);

private:
                                                Logger          (const Logger&);    // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            Logger&             operator=       (const Logger&);    // Intentionally undefined. Is private because we do not allow copies of a Singleton.

	static                  Logger*             g_defaultInstance;  // A pointer to the default logger. This is not a singleton pointer.
	                        bool                m_active;
	                        SeverityLevel       m_logLevel;         // The log level allows to filter messages, and only log those above the given severity threshold

}; // class Logger

} // namespace nxn


//#include <filesystem>

#define WriteLogMessage(level, msg) nxn::Logger::GetLogger()->WriteMessage  (msg,   level, std::filesystem::path(__FILE__).filename().string(), __LINE__);
#define GetLogStream(level)	        nxn::Logger::GetLogger()->GetStream     (       level, std::filesystem::path(__FILE__).filename().string(), __LINE__)

//#define LogMessage(level, msg)
//#define GetLogStream(level)       nxn::Logger::g_nullStream


#endif // !NXN_LOGGER
