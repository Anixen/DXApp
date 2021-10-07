/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include "pch.h"

#include "../../lib-DXApp/src/LoggerConsole.h"
#include "AppSimpleCompute.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	LoggerConsole logger(true, SeverityDebug);

    // Renders only 2D, so no need for a depth buffer.
    std::unique_ptr<DX::DeviceResources> deviceResources = std::make_unique<DX::DeviceResources>(
        DXGI_FORMAT_B8G8R8A8_UNORM, // backBufferFormat
        DXGI_FORMAT_UNKNOWN,        // depthBufferFormat
        2,                          // backBufferCount
        D3D_FEATURE_LEVEL_11_0      // minFeatureLevel
    );

    App* app = new AppSimpleCompute(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);
	int exitCode = app->Run();

	delete app;

	return exitCode;
}
