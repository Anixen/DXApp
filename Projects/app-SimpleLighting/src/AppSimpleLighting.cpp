#include "pch.h"
#include "AppSimpleLighting.h"

#include "AppStateSimpleLighting.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

    AppSimpleLighting::AppSimpleLighting()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

    AppSimpleLighting::~AppSimpleLighting()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::preInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleLighting(this));
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
