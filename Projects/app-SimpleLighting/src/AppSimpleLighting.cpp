#include "pch.h"
#include "AppSimpleLighting.h"

#include "AppStateSimpleLighting.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleLighting::AppSimpleLighting(std::unique_ptr<DX::DeviceResources> & p_deviceResources)
    : App(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleLighting::~AppSimpleLighting()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleLighting(this));
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleLighting::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleLighting::HandleCleanup()" << std::endl;
}

} // namespace nxn
