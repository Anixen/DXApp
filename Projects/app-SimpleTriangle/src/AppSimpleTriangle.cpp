#include "pch_SimpleTriangle.h"
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

void AppSimpleTriangle::preInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::preInit()" << std::endl;
    //*/

    setCurrentState(new AppStateSimpleTriangle(this));
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle::postInit()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::postInit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppSimpleTriangle::handleCleanUp()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppSimpleTriangle::handleCleanup()" << std::endl;
    //*/
}

} // namespace nxn
