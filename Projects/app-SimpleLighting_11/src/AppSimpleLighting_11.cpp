#include "pch.h"
#include "AppSimpleLighting_11.h"

#include "AppStateSimpleLighting_11.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleLighting_11::AppSimpleLighting_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleLighting_11::~AppSimpleLighting_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleLighting_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
