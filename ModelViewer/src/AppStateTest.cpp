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

AppState* AppStateTest::update(double p_elapsedSeconds)
{
	/*
	GetLogStream(SeverityInfo)
		<< "AppStateTest::update(" << p_elapsedSeconds << ")" << std::endl;
	//*/

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateTest::draw()
{
	Sleep(15);
}

//-----------------------------------------------------------------------------

void AppStateTest::handleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::handleCleanup()" << std::endl;
}

} // namespace nxn