#include "pch.h"
#include "AppSimpleInstancing_11.h"

#include "AppStateSimpleInstancing_11.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleInstancing_11::AppSimpleInstancing_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleInstancing_11::~AppSimpleInstancing_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_11::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleInstancing_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
