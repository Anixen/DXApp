#include "pch.h"
#include "AppSimpleCompute_12.h"

#include "AppStateSimpleCompute_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleCompute_12::AppSimpleCompute_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleCompute_12::~AppSimpleCompute_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_12::GetDefaultWindowSize(int & width, int & height) const
{
    width = 1280;
    height = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleCompute_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
