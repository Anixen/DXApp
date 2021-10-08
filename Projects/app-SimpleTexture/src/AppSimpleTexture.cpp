#include "pch.h"
#include "AppSimpleTexture.h"

#include "AppStateSimpleTexture.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTexture::AppSimpleTexture(std::unique_ptr<DX::DeviceResources> & p_deviceResources)
    : App(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleTexture::~AppSimpleTexture()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleTexture(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture::HandleCleanup()" << std::endl;
}

} // namespace nxn
