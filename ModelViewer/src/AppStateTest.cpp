#include "pch.h"
#include "AppStateTest.h"

#include "Logger.h"
#include "App.h"


namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateTest::AppStateTest(App *p_app) :
	AppState(p_app)
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateTest::~AppStateTest()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateTest::init() {
	GetLogStream(SeverityInfo)
		<< "AppStateTest::init()" << std::endl;

	AppState::init();
}

//-----------------------------------------------------------------------------

void AppStateTest::reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState* AppStateTest::update(DX::StepTimer const& p_timer,
    DirectX::GamePad* p_gamePad, DirectX::Keyboard* p_keyboard, DirectX::Mouse* p_mouse)
{
    (void)p_gamePad;
    (void)p_keyboard;
    (void)p_mouse;

    double elapsedSeconds = p_timer.GetElapsedSeconds();
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateTest::update(" << elapsedSeconds << ")" << std::endl;
	//*/

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateTest::draw(DX::DeviceResources* p_deviceResources)
{
    (void)p_deviceResources;
	Sleep(15);
}

//-----------------------------------------------------------------------------

void AppStateTest::clear(DX::DeviceResources * p_deviceResources)
{
    (void)p_deviceResources;
}

//-----------------------------------------------------------------------------

void AppStateTest::handleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::handleCleanup()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateTest::createDeviceDependentResources(DX::DeviceResources * p_deviceResources)
{
    (void)p_deviceResources;
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle::createDeviceDependentResources()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateTest::createWindowSizeDependentResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle::createWindowSizeDependentResources()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateTest::resetResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle::resetRessources()" << std::endl;
}

} // namespace nxn