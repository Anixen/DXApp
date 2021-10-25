#include "pch.h"
#include "AppSimpleLighting_12.h"

#include "AppStateSimpleLighting_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleLighting_12::AppSimpleLighting_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleLighting_12::~AppSimpleLighting_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleLighting_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
