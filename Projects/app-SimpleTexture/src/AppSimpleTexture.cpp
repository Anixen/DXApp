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
        << "AppSimpleTexture::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppSimpleTexture::~AppSimpleTexture()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::PreInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleTexture(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::PostInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::HandleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
