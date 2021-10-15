#include "pch.h"
#include "AppSimpleTexture_11.h"

#include "AppStateSimpleTexture_11.h"
#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTexture_11::AppSimpleTexture_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleTexture_11::~AppSimpleTexture_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleTexture_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTexture_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTexture_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
