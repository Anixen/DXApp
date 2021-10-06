/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include "pch.h"

#include "../../lib-DXApp/src/LoggerConsole.h"
#include "AppSimpleTexture.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	LoggerConsole logger(true, SeverityDebug);

    App* app = new AppSimpleTexture();

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);
	int exitCode = app->Run();

	delete app;

	return exitCode;
}

/*
int main()
{
	LoggerConsole logger(true, SeverityDebug);

	Timer timer;
	timer.init();

	Sleep(20);

	// We should have a little bit (< 1 ms) more than the sleep duration
	float elaspedTime = timer.getElapsedTime();
	GetLogStream(SeverityDebug) << "elapsedTime : " << std::to_string(elaspedTime) << std::endl;
}
//*/


/*
int main()
{
	//std::cout << "Hello world !" << std::endl;

	LoggerConsole logger(true, SeverityDebug);

	LogMessage(SeverityDebug,	"Test Debug");
	//logger.setLogLevel(SeverityWarning);
	LogMessage(SeverityInfo,	"Test Info");
	LogMessage(SeverityWarning, "Test Warning");
	LogMessage(SeverityError,	"Test Error");
	LogMessage(SeverityFatal,	"Test Fatal");

	return 0;
}
//*/

/*
int main()
{
	std::cout << "Hello world !" << std::endl;
}
//*/