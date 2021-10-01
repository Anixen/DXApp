#include "pch.h"
#include "AppSamples.h"

//#include "AppStateSimpleTriangle.h"
#include "AppStateSimpleTexture.h"
#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSamples::AppSamples()
{
    GetLogStream(SeverityInfo)
        << "AppSamples::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSamples::~AppSamples()
{
    GetLogStream(SeverityInfo)
        << "AppSamples::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSamples::preInit()
{
    GetLogStream(SeverityInfo)
        << "AppSamples::preInit()" << std::endl;

    setCurrentState(new AppStateSimpleTexture(this));
}

//-----------------------------------------------------------------------------

void AppSamples::postInit()
{
    GetLogStream(SeverityInfo)
        << "AppSamples::postInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSamples::handleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSamples::handleCleanup()" << std::endl;
}

} // namespace nxn
