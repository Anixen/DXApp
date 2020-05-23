/**
 * @file Base class for the logging system
 *
 * @author Olivier Falconnet
 */

#pragma once

#include <string>
#include <ostream>

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
	// TODO : Implement onnullstream
	//static std::onnullstream _nullStream;

	/**
	 * LoggerBase deconstructor
	 */
	~LoggerBase();


	/**
	 * @return {ILogger*} A pointer to the most recently instantiated logger
	 */
	static LoggerBase* getLogger();

	/**
	 * @return {bool} true if the logger is active, false if not
	 */
	bool isActive();

	/**
	 * Sets if the logger is active or not
	 *
	 * @param {bool} p_active Set to true to activate the logger, false to deactivate
	 */
	void setActive(bool p_active);

	/**
	 * @return {SeverityLevel} The log level set for the logger
	 */
	SeverityLevel getLogLevel();

	/**
	 * Sets the log level for the logger
	 * The log level allows to filter messages, and only log those above a given severity threshold
	 *  
	 * @param {SeverityLevel} p_logLevel The log level for the logger
	 */
	void setLogLevel(SeverityLevel p_logLevel);

	/**
	 * @return {std::ostream&} A reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream() = 0;

	/**
	 * Returns a reference to the stream and inserts a timestamp and File:Line tag inside
	 *
	 * @param {SeverityLevel} p_severityLevel The severity level for the message to log
	 * @param {std::string} p_sourceFile The source file where the logger has been called from
	 * @param {int} p_sourceLine The line number where the logger has been called from
	 * 
	 * @return {std::ostream&} A reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream(SeverityLevel p_severityLevel,
		std::string p_sourceFile, int p_sourceLine) = 0;


	/**
	 * Logs the provided message
	 *
	 * @param {std::string} p_message The message to log
	 */
	virtual void logMessage(const std::string &p_message) = 0;

	/**
	 * Logs the provided message with a timestamp and File:Line tag in front
	 *
	 * @param {std::string} p_message The message to log
	 * @param {SeverityLevel} p_severityLevel The severity level for the message to log
	 * @param {std::string} p_sourceFile The source file where the logger has been called from
	 * @param {int} p_sourceLine The line number where the logger has been called from
	 */
	virtual void logMessage(const std::string &p_message, SeverityLevel p_severityLevel,
		std::string p_sourceFile, int p_sourceLine) = 0;

protected:
	/**
	 * LoggerBase constructor
	 * Is protected because we only allow derived classes to instantiate this interface
	 *
	 * @param {bool} p_makeDefault true if the new logger must become the default logger, false if not
	 * @param {SeverityLevel} p_logLevel The log level for the logger
	 */
	LoggerBase(bool p_makeDefault, SeverityLevel p_logLevel = SeverityInfo);

	/**
	 * @returns {bool} true if p_severityLevel is above or equal to the log level for the logger, false if not
	 */
	bool shouldLog(SeverityLevel p_severityLevel);

	/**
	 * Write a timestamp and File:Line tag to the provided ostream
	 * @param {std::ostream} p_ostream The ostream where to write the prefix tag
	 * @param {SeverityLevel} p_severityLevel The severity level for the message to log
	 * @param {std::string} p_sourceFile The source file where the logger has been called from
	 * @param {int} p_sourceLine The line number where the logger has been called from
	 */
	void writeTag(std::ostream &p_ostream, SeverityLevel p_severityLevel,
		std::string p_sourceFile, int p_sourceLine);

private:
	static LoggerBase* g_instance;

	bool m_active;
	SeverityLevel m_logLevel;

	/**
	 * LoggerBase copy constructor
	 * is private because we do not allow copies of a Singleton
	 */
	LoggerBase(const LoggerBase&);  // Intentionally undefined

	/**
	 * LoggerBase assignment operator
	 * is private because we do not allow copies of a Singleton
	 */
	LoggerBase& operator=(const LoggerBase&); // Intentionally undefined

}; // class LoggerBase


#include <filesystem>

#define LogMessage(level, msg) LoggerBase::getLogger()->logMessage(msg, level, std::filesystem::path(__FILE__).filename().string(), __LINE__);
#define GetLogStream(level)	LoggerBase::getLogger()->getStream(level, std::filesystem::path(__FILE__).filename().string(), __LINE__)