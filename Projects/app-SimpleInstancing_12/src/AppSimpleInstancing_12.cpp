#include "pch.h"
#include "AppSimpleInstancing_12.h"

#include "AppStateSimpleInstancing_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleInstancing_12::AppSimpleInstancing_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleInstancing_12::~AppSimpleInstancing_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_12::GetDefaultWindowSize(int & width, int & height) const
{
    width = 1280;
    height = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleInstancing_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
