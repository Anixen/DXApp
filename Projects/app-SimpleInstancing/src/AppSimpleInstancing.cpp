#include "pch.h"
#include "AppSimpleInstancing.h"

#include "AppStateSimpleInstancing.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleInstancing::AppSimpleInstancing()
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
        << "AppSimpleInstancing::preInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleInstancing(this));
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleInstancing::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleInstancing::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
