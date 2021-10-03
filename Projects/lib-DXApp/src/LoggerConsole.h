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

#include "Logger.h"


namespace nxn {


class LoggerConsole : public Logger {

public:
	                                        LoggerConsole   (bool p_makeDefault, SeverityLevel p_logLevel = SeverityInfo);
	                                        ~LoggerConsole  ();

    virtual                 std::ostream&   GetStream       () const;
	virtual                 std::ostream&   GetStream       (   SeverityLevel p_severityLevel,
	                                                            std::string p_sourceFile, int p_sourceLine) const;

	virtual                 void            WriteMessage    (   const std::string &p_message) const;
	virtual                 void            WriteMessage    (   const std::string &p_message,
                                                                SeverityLevel p_severityLevel,
		                                                        std::string p_sourceFile, int p_sourceLine) const;

private:
	                                        LoggerConsole   (const LoggerConsole&); // Intentionally undefined. Is private because we do not allow copying items of this class.
	                        LoggerConsole&  operator=       (const LoggerConsole&); // Intentionally undefined. Is private because we do not allow copying items of this class.

}; // class LoggerConsole

} // namespace nxn
