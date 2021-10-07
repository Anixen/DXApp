/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

//#include <iostream>

#include "pch.h"

#include "../../lib-DXApp/src/LoggerConsole.h"
#include "AppSimpleTriangle.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	LoggerConsole logger(true, SeverityDebug);

    App* app = new AppSimpleTriangle();

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);
	int exitCode = app->Run();

	delete app;

	return exitCode;
}
