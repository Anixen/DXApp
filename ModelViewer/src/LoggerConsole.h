/**
 * @file Console implementation for the logging system
 *
 * @author Olivier Falconnet
 */

#pragma once

#include "LoggerBase.h"

class LoggerConsole : public LoggerBase {

public:

	/**
	 * LoggerConsole constructor
	 *
	 * @param {bool} pMakeDefault True if the new logger must become the default logger, false if not
	 * @param {SeverityLevel} pLogLevel The log level for the logger
	 */
	LoggerConsole(bool pMakeDefault, SeverityLevel pLogLevel = SeverityInfo);

	/**
	 * LoggerConsole deconstructor
	 */
	~LoggerConsole();


	/**
	 *  @return {std::ostream&} a reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream();

	/**
	 * Returns a reference to the stream and inserts a timestamp and File:Line tag inside
	 *
	 * @return {std::ostream&} a reference to the ostream where the logger sends the messages that are given to him
	 */
	virtual std::ostream& getStream(
		SeverityLevel pSeverityLevel,
		std::string pSourceFile, int pSourceLine);


	/**
	 * Logs the provided message
	 *
	 * @param {std::string} pMessage The message to log
	 */
	virtual void logMessage(const std::string &pMessage);

	/**
	 * Logs the provided message with a timestamp and File:Line tag in front
	 *
	 * @param {std::string} pMessage The message to log
	 * @param {SeverityLevel} pSeverityLevel The severity level for the message to log
	 * @param {std::string} pSourceFile The source file where the logger has been called from
	 * @param {int} pSourceLine The line number where the logger has been called from
	 */
	virtual void logMessage(
		const std::string &pMessage, SeverityLevel pSeverityLevel,
		std::string pSourceFile, int pSourceLine);

protected:

private:
	/**
	 * ConsoleLogger copy constructor
	 * is private because we do not allow copies of a Singleton
	 */
	LoggerConsole(const LoggerConsole&);  // Intentionally undefined

	/**
	 * ConsoleLogger assignment operator
	 * is private because we do not allow copies of a Singleton
	 */
	LoggerConsole& operator=(const LoggerConsole&); // Intentionally undefined

}; // class LoggerConsole