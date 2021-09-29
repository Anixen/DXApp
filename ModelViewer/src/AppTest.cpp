#include "pch.h"
#include "AppTest.h"

#include "AppStateTest.h"
#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppTest::AppTest()
{
	GetLogStream(SeverityInfo)
		<< "AppTest::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppTest::~AppTest()
{
	GetLogStream(SeverityInfo)
		<< "AppTest::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppTest::init()
{
	GetLogStream(SeverityInfo)
		<< "AppTest::init()" << std::endl;

	setCurrentState(new AppStateTest(this));
}

//-----------------------------------------------------------------------------

void AppTest::handleCleanUp()
{
	GetLogStream(SeverityInfo)
		<< "TestApp::cleanup()" << std::endl;
}

} // namespace nxn
