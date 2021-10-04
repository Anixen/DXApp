#include "pch.h"
#include "AppSimpleBezier.h"

#include "AppStateSimpleBezier.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

    AppSimpleBezier::AppSimpleBezier()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

    AppSimpleBezier::~AppSimpleBezier()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::preInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleBezier(this));
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
