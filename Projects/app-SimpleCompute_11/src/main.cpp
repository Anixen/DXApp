/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

#include "pch.h"

#include "LoggerConsole.h"
#include "AppSimpleCompute_11.h"


using namespace nxn;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr))
        return 1;

	LoggerConsole logger(true, SeverityDebug);

    // Renders only 2D, so no need for a depth buffer.
    std::unique_ptr<DX::DeviceResources_11> deviceResources = std::make_unique<DX::DeviceResources_11>(
        DXGI_FORMAT_B8G8R8A8_UNORM, // backBufferFormat
        DXGI_FORMAT_UNKNOWN,        // depthBufferFormat
        2,                          // backBufferCount
        D3D_FEATURE_LEVEL_11_0      // minFeatureLevel
    );

    std::unique_ptr<App_11> app = std::make_unique<AppSimpleCompute_11>(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);

    //app->SetFixedUpdate(true);
    //app->SetUpdateFrequency(200);

	int exitCode = app->Run();

    app.reset();

    CoUninitialize();

	return exitCode;
}
