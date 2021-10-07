#include "pch.h"
#include "AppSimpleCompute.h"

#include "AppStateSimpleCompute.h"
#include "../../lib-DXApp/src/Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleCompute::AppSimpleCompute()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppSimpleCompute::~AppSimpleCompute()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleCompute::GetDefaultWindowSize(int & width, int & height) const
{
    width   = 1280;
    height  = 720;
}

//-----------------------------------------------------------------------------

void AppSimpleCompute::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute::PreInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleCompute(this));
}

//-----------------------------------------------------------------------------

void AppSimpleCompute::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute::PostInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleCompute::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleCompute::HandleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
