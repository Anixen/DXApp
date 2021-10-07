/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include "pch.h"

#include "../../lib-DXApp/src/LoggerConsole.h"
#include "AppSimpleInstancing.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	LoggerConsole logger(true, SeverityDebug);

    // Use gamma-correct rendering. Requires Feature Level 10.0 or greater.
    std::unique_ptr<DX::DeviceResources> deviceResources = std::make_unique<DX::DeviceResources>(
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,    // backBufferFormat
        DXGI_FORMAT_D32_FLOAT,              // depthBufferFormat
        2,                                  // backBufferCount
        D3D_FEATURE_LEVEL_10_0              // minFeatureLevel
    );            

    App* app = new AppSimpleInstancing(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);
	int exitCode = app->Run();

	delete app;

	return exitCode;
}
