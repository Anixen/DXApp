#include "pch_SimpleTexture.h"
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

void AppSimpleTexture::preInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::preInit()" << std::endl;
    //*/

    setCurrentState(new AppStateSimpleTexture(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::postInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::handleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSamples::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
