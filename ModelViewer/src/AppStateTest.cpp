#include "AppStateTest.h"

#include "LoggerBase.h"
#include "AppBase.h"


AppStateTest::AppStateTest(AppBase *pApp) :
	AppStateBase(pApp)
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::ctor()" << std::endl;
}

AppStateTest::~AppStateTest()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::dtor()" << std::endl;
}

void AppStateTest::init() {
	GetLogStream(SeverityInfo)
		<< "AppStateTest::init()" << std::endl;

	AppStateBase::init();
}

void AppStateTest::reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::reinit()" << std::endl;
}

void AppStateTest::updateFixed()
{
	/*
	GetLogStream(SeverityInfo)
		<< "AppStateTest::updateFixed()" << std::endl;
	//*/
}

void AppStateTest::updateVariable(float elapsedTime)
{
	/*
	GetLogStream(SeverityInfo)
		<< "AppStateTest::updateVariable(" << elapsedTime << ")" << std::endl;
	//*/
}

void AppStateTest::draw()
{
	Sleep(15);
}

void AppStateTest::handleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateTest::handleCleanup()" << std::endl;
}