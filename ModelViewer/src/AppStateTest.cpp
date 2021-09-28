#include "AppStateTest.h"

#include "LoggerBase.h"
#include "AppBase.h"


//-----------------------------------------------------------------------------
/**
 * @param {AppBase*}    p_app  The address of the app to which belongs this state
 */
AppStateTest::AppStateTest(AppBase *p_app) :
	AppStateBase(p_app)
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

	AppStateBase::init();
}

//-----------------------------------------------------------------------------

void AppStateTest::reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateBase* AppStateTest::update(double p_elapsedSeconds)
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
