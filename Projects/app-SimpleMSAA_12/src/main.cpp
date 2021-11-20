/**
 * @file Entry point for the programm
 *
 * @author Olivier Falconnet
 */

//#include <iostream>

#include "pch.h"

#include "LoggerConsole.h"
#include "AppSimpleMSAA_12.h"


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
        c_backBufferFormat,
        c_depthBufferFormat,    /* If we were only doing MSAA rendering, we could skip the non-MSAA depth/stencil buffer with DXGI_FORMAT_UNKNOWN */
        2                       // backBufferCount
    );
    //
    // In Win32 'classic' DirectX 11, you can create the 'swapchain' backbuffer as a multisample buffer.  
    // Present took care of the resolve as part of the swapchain management.  
    // This approach is not recommended as doing it explictly gives you more control
    // as well as the fact that this 'old-school' implicit resolve behavior is not supported for UWP or DirectX 12.
    //

    std::unique_ptr<App_12> app = std::make_unique<AppSimpleMSAA_12>(deviceResources);

	//app->processArguments(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->ProcessArguments(argc, argv);

    //app->SetFixedUpdate(true);
    //app->SetUpdateFrequency(200);

	int exitCode = app->Run();

    app.reset();

    CoUninitialize();

	return exitCode;
}
