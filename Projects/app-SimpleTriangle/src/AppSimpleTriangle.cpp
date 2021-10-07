#include "pch.h"
#include "AppSimpleTriangle.h"

#include "AppStateSimpleTriangle.h"


namespace nxn {


//-----------------------------------------------------------------------------

AppSimpleTriangle::AppSimpleTriangle()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppSimpleTriangle::~AppSimpleTriangle()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle::PreInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::PreInit()" << std::endl;
    //*/

    SetCurrentState(new AppStateSimpleTriangle(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle::PostInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::PostInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle::HandleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::HandleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
