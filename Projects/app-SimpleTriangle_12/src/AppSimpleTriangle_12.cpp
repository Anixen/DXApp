#include "pch.h"
#include "AppSimpleTriangle_12.h"

#include "AppStateSimpleTriangle_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTriangle_12::AppSimpleTriangle_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleTriangle_12::~AppSimpleTriangle_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleTriangle_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
