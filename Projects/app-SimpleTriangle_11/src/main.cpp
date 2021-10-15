/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

//#include <iostream>

#include "pch.h"

#include "LoggerConsole.h"
#include "AppSimpleTriangle_11.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	LoggerConsole logger(true, SeverityDebug);

    // Use gamma-correct rendering.
    std::unique_ptr<DX::DeviceResources_11> deviceResources = std::make_unique<DX::DeviceResources_11>(
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB // backBufferFormat
    );

    App_11* app = new AppSimpleTriangle_11(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);

    //app->SetFixedUpdate(true);
    //app->SetUpdateFrequency(200);

	int exitCode = app->Run();

	delete app;

	return exitCode;
}
