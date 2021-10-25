#include "pch.h"
#include "AppSimpleBezier_12.h"

#include "AppStateSimpleBezier_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleBezier_12::AppSimpleBezier_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleBezier_12::~AppSimpleBezier_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleBezier_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
