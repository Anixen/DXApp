/**
 * Console implementation for the logging system
 *
 * @class LoggerConsole
 *
 *
 * @file src/LoggerConsole.h
 * @author Olivier Falconnet
 * @date 20200521 - File creation
 * @date 20210927 - Updated coding style
 */

#pragma once

#include "LoggerBase.h"


class LoggerConsole : public LoggerBase {

public:
	                                        LoggerConsole   (bool p_makeDefault, SeverityLevel p_logLevel = SeverityInfo);
	                                        ~LoggerConsole  ();

    virtual                 std::ostream&   getStream       () const;
	virtual                 std::ostream&   getStream       (   SeverityLevel p_severityLevel,
	                                                            std::string p_sourceFile, int p_sourceLine) const;

	virtual                 void            logMessage      (   const std::string &p_message) const;
	virtual                 void            logMessage      (   const std::string &p_message,
                                                                SeverityLevel p_severityLevel,
		                                                        std::string p_sourceFile, int p_sourceLine) const;

private:
	                                        LoggerConsole   (const LoggerConsole&); // Intentionally undefined. Is private because we do not allow copying items of this class.
	                        LoggerConsole&  operator=       (const LoggerConsole&); // Intentionally undefined. Is private because we do not allow copying items of this class.

}; // class LoggerConsole
