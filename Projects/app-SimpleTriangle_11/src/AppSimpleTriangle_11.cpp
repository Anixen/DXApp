#include "pch.h"
#include "AppSimpleTriangle_11.h"

#include "AppStateSimpleTriangle_11.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTriangle_11::AppSimpleTriangle_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleTriangle_11::~AppSimpleTriangle_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleTriangle_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
