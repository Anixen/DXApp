#include "AppStateBase.h"

#include "LoggerBase.h"
#include "AppBase.h"


AppStateBase::AppStateBase(AppBase* pApp, std::string pID) :
	mApp(pApp),
	mInitialized(false),
	mCleanup(false),
	mID(pID),
	mElapsedTime(0),
	mTotalPausedTime(0),
	mElapsedClock(),
	mPausedClock()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::ctor()" << std::endl;
}

AppStateBase::~AppStateBase()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::dtor()" << std::endl;
}

std::string AppStateBase::getID() const
{
	return mID;
}

void AppStateBase::init()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::init() [\"" << mID << "\"]" << std::endl;

	if (mCleanup) {
		handleCleanup();
	}
	//*/

	if (!mInitialized) {

		mPaused = false;
		mElapsedTime = 0;
		mTotalPausedTime = 0;

		mElapsedClock.reset();
		mPausedClock.reset();

		mInitialized = true;
	}
}

void AppStateBase::deinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::deinit() [" << mID << "]" << std::endl;

	if (mInitialized) {

		mCleanup = true;

		mElapsedTime += mElapsedClock.getElapsedTime();

		if (mPaused) {
			mTotalPausedTime += mPausedClock.getElapsedTime();
		}

		mInitialized = false;
	}
}

bool AppStateBase::isInitialized() const
{
	return mInitialized;
}

void AppStateBase::pause()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::pause() [" << mID << "]" << std::endl;

	if (!mPaused) {

		mPausedClock.reset();
		mPaused = true;
	}
}

void AppStateBase::resume()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::resume() [" << mID << "]" << std::endl;

	if (mPaused) {

		mTotalPausedTime += mPausedClock.getElapsedTime();
		mPaused = false;
	}
}

bool AppStateBase::isPaused() const
{
	return mPaused;
}

void AppStateBase::cleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::cleanup() [" << mID << "]" << std::endl;

	if (mCleanup) {

		handleCleanup();
		mCleanup = false;
	}
}

float AppStateBase::getElapsedTime() const
{
	return (mInitialized) ? mElapsedClock.getElapsedTime() : mElapsedTime;
}