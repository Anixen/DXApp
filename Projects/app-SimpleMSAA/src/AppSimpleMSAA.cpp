#include "pch.h"
#include "AppSimpleMSAA.h"

#include "AppStateSimpleMSAA.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleMSAA::AppSimpleMSAA()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppSimpleMSAA::~AppSimpleMSAA()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA::PreInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleMSAA(this));
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA::PostInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleMSAA::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleMSAA::HandleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
