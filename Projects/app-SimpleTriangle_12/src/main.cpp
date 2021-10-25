/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

//#include <iostream>

#include "pch.h"

#include "LoggerConsole.h"
#include "AppSimpleTriangle_12.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    if (FAILED(initialize))
        return 1;

	LoggerConsole logger(true, SeverityDebug);

    // Use gamma-correct rendering.
    std::unique_ptr<DX::DeviceResources_12> deviceResources = std::make_unique<DX::DeviceResources_12>(
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,        // backBufferFormat
        DXGI_FORMAT_D32_FLOAT,                  // depthBufferFormat
        2,                                      // backBufferCount
        D3D_FEATURE_LEVEL_11_0,                 // minFeatureLevel
        DX::DeviceResources_12::c_AllowTearing  // flags
    );

    std::unique_ptr<App_12> app = std::make_unique<AppSimpleTriangle_12>(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);

    //app->SetFixedUpdate(true);
    //app->SetUpdateFrequency(200);

	int exitCode = app->Run();

    app.reset();

    CoUninitialize();

	return exitCode;
}
