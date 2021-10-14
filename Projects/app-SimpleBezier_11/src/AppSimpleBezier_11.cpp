#include "pch.h"
#include "AppSimpleBezier_11.h"

#include "AppStateSimpleBezier_11.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleBezier_11::AppSimpleBezier_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleBezier_11::~AppSimpleBezier_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleBezier_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
