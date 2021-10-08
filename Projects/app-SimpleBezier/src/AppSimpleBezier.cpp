#include "pch.h"
#include "AppSimpleBezier.h"

#include "AppStateSimpleBezier.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleBezier::AppSimpleBezier(std::unique_ptr<DX::DeviceResources> & p_deviceResources)
    : App(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleBezier::~AppSimpleBezier()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleBezier(this));
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleBezier::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleBezier::HandleCleanup()" << std::endl;
}

} // namespace nxn
