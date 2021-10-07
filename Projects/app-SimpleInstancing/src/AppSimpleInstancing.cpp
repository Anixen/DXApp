#include "pch.h"
#include "AppSimpleInstancing.h"

#include "AppStateSimpleInstancing.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleInstancing::AppSimpleInstancing(std::unique_ptr<DX::DeviceResources> & p_deviceResources)
    : App(p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppSimpleInstancing::~AppSimpleInstancing()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::PreInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleInstancing(this));
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::PostInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::HandleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
