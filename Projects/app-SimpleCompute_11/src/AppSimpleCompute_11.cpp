#include "pch.h"
#include "AppSimpleCompute_11.h"

#include "AppStateSimpleCompute_11.h"
#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleCompute_11::AppSimpleCompute_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleCompute_11::~AppSimpleCompute_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_11::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleCompute_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
