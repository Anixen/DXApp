#include "pch.h"
#include "AppSimpleTexture.h"

#include "AppStateSimpleTexture.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

    AppSimpleTexture::AppSimpleTexture()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

    AppSimpleTexture::~AppSimpleTexture()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::preInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleTexture(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
