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
	 * @param {bool} pActive Set to true to activate the logger, false to deactivate
	 */
	void setActive(bool pActive);

	/**
	 * @return {SeverityLevel} The log level set for the logger
	 */
	SeverityLevel getLogLevel();

	/**
	 * Sets the log level for the logger
	 * The log level allows to filter messages, and only log those above a given severity threshold
	 *  
	 * @param {SeverityLevel} pLogLevel The log level for the logger
	 */
	void setLogLevel(SeverityLevel pLogLevel);

	/**
	 * @return {std::ostream&} A reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream() = 0;

	/**
	 * Returns a reference to the stream and inserts a timestamp and File:Line tag inside
	 *
	 * @param {SeverityLevel} pSeverityLevel The severity level for the message to log
	 * @param {std::string} pSourceFile The source file where the logger has been called from
	 * @param {int} pSourceLine The line number where the logger has been called from
	 * 
	 * @return {std::ostream&} A reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream(SeverityLevel pSeverityLevel,
		std::string pSourceFile, int pSourceLine) = 0;


	/**
	 * Logs the provided message
	 *
	 * @param {std::string} pMessage The message to log
	 */
	virtual void logMessage(const std::string &pMessage) = 0;

	/**
	 * Logs the provided message with a timestamp and File:Line tag in front
	 *
	 * @param {std::string} pMessage The message to log
	 * @param {SeverityLevel} pSeverityLevel The severity level for the message to log
	 * @param {std::string} pSourceFile The source file where the logger has been called from
	 * @param {int} pSourceLine The line number where the logger has been called from
	 */
	virtual void logMessage(const std::string &pMessage, SeverityLevel pSeverityLevel,
		std::string pSourceFile, int pSourceLine) = 0;

protected:
	/**
	 * LoggerBase constructor
	 * Is protected because we only allow derived classes to instantiate this interface
	 *
	 * @param {bool} pMakeDefault true if the new logger must become the default logger, false if not
	 * @param {SeverityLevel} pLogLevel The log level for the logger
	 */
	LoggerBase(bool pMakeDefault, SeverityLevel pLogLevel = SeverityInfo);

	/**
	 * @returns {bool} true if pSeverityLevel is above or equal to the log level for the logger, false if not
	 */
	bool shouldLog(SeverityLevel pSeverityLevel);

	/**
	 * Write a timestamp and File:Line tag to the provided ostream
	 * @param {std::ostream} pOstream The ostream where to write the prefix tag
	 * @param {SeverityLevel} pSeverityLevel The severity level for the message to log
	 * @param {std::string} pSourceFile The source file where the logger has been called from
	 * @param {int} pSourceLine The line number where the logger has been called from
	 */
	void writeTag(std::ostream &pOstream, SeverityLevel pSeverityLevel,
		std::string pSourceFile, int pSourceLine);

private:
	static LoggerBase* gInstance;

	bool mActive;
	SeverityLevel mLogLevel;

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