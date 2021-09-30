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

void AppTest::preInit()
{
    GetLogStream(SeverityInfo)
        << "AppTest::preInit()" << std::endl;

    setCurrentState(new AppStateTest(this));
}

//-----------------------------------------------------------------------------

void AppTest::postInit()
{
    GetLogStream(SeverityInfo)
        << "AppTest::postInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppTest::handleCleanUp()
{
	GetLogStream(SeverityInfo)
		<< "AppTest::cleanup()" << std::endl;
}

} // namespace nxn
