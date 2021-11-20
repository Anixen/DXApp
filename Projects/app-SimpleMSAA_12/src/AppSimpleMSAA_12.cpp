#include "pch.h"
#include "AppSimpleMSAA_12.h"

#include "AppStateSimpleMSAA_12.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleMSAA_12::AppSimpleMSAA_12(std::unique_ptr<DX::DeviceResources_12> & p_deviceResources)
    : App_12(p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppSimpleMSAA_12::~AppSimpleMSAA_12()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_12::GetDefaultWindowSize(int & width, int & height) const
{
    width = 1280;
    height = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_12::PreInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_12::PreInit()" << std::endl;

    SetCurrentState(new AppStateSimpleMSAA_12(this));
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_12::PostInit()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_12::PostInit()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA_12::HandleCleanUp()
{
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA_12::HandleCleanup()" << std::endl;
}

} // namespace nxn
