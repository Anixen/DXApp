#include "AppStateBase.h"

#include "LoggerBase.h"
#include "AppBase.h"


namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {AppBase*}    p_app  The address of the app to which belongs this state
 */
AppStateBase::AppStateBase(AppBase* p_app) :
	m_app(p_app),
	m_initialized(false),
	m_cleanup(false),
	m_elapsedTime(0),
	m_totalPausedTime(0),
	m_elapsedClock(),
	m_pausedClock()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateBase::~AppStateBase()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------
/**
 * Initializes the state and make it do some clean-up if needed
 */
void AppStateBase::init()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::init()" << std::endl;

	if (m_cleanup) {
		handleCleanup();
	}
	//*/

	if (!m_initialized) {

		m_paused = false;
		m_elapsedTime = 0;
		m_totalPausedTime = 0;

		m_elapsedClock.reset();
		m_pausedClock.reset();

		m_initialized = true;
	}
}

//-----------------------------------------------------------------------------
/**
 * Un-Initializes a state and marks it for clean-up
 * This function should be called whenever a state becomes inactive.
 */
void AppStateBase::deinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::deinit()" << std::endl;

	if (m_initialized) {

		m_cleanup = true;

		m_elapsedTime += m_elapsedClock.getElapsedTime();

		if (m_paused) {
			m_totalPausedTime += m_pausedClock.getElapsedTime();
		}

		m_initialized = false;
	}
}

//-----------------------------------------------------------------------------

void AppStateBase::pause()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::pause()" << std::endl;

	if (!m_paused) {

		m_pausedClock.reset();
		m_paused = true;
	}
}

//-----------------------------------------------------------------------------

void AppStateBase::resume()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::resume()" << std::endl;

	if (m_paused) {

		m_totalPausedTime += m_pausedClock.getElapsedTime();
		m_paused = false;
	}
}

//-----------------------------------------------------------------------------

void AppStateBase::cleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateBase::cleanup()" << std::endl;

	if (m_cleanup) {

		handleCleanup();
		m_cleanup = false;
	}
}

//-----------------------------------------------------------------------------

float AppStateBase::getElapsedTime() const
{
	return (m_initialized) ? m_elapsedClock.getElapsedTime() : m_elapsedTime;
}

} // namespace nxn
