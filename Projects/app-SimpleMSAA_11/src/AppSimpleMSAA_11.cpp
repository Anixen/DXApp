#include "pch.h"
#include "AppSimpleMSAA_11.h"

#include "AppStateSimpleMSAA_11.h"
#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleMSAA_11::AppSimpleMSAA_11(std::unique_ptr<DX::DeviceResources_11> & p_deviceResources)
    : App_11(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleMSAA_11::~AppSimpleMSAA_11()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_11::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_11::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_11::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleMSAA_11(this));
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_11::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_11::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_11::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_11::HandleCleanup()" << std::endl;
}

} // namespace nxn
