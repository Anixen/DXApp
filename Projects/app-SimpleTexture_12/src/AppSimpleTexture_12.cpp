#include "pch.h"
#include "AppSimpleTexture_12.h"

#include "AppStateSimpleTexture_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTexture_12::AppSimpleTexture_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleTexture_12::~AppSimpleTexture_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleTexture_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
