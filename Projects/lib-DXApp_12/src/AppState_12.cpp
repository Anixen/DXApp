#include "pch.h"
#include "AppState_12.h"

#include "Logger.h"


namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App_12*}    p_app  The address of the app to which belongs this state
 */
AppState_12::AppState_12(App_12* p_app) :
	m_app(p_app),
	m_initialized(false),
	m_cleanup(false)
{
	GetLogStream(SeverityInfo)
		<< "AppState::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState_12::~AppState_12()
{
	GetLogStream(SeverityInfo)
		<< "AppState::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------
/**
 * Initializes the state and make it do some clean-up if needed
 */
void AppState_12::Init()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Init()" << std::endl;

	if (m_cleanup) {
		HandleCleanup();
	}
	//*/

	if (!m_initialized) {

		m_paused = false;
		m_initialized = true;
	}
}

//-----------------------------------------------------------------------------
/**
 * Un-Initializes a state and marks it for clean-up
 * This function should be called whenever a state becomes inactive.
 */
void AppState_12::Deinit()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Deinit()" << std::endl;

	if (m_initialized) {

		m_cleanup = true;
		m_initialized = false;
	}
}

//-----------------------------------------------------------------------------

void AppState_12::Pause()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Pause()" << std::endl;

	if (!m_paused) {

		m_paused = true;
	}
}

//-----------------------------------------------------------------------------

void AppState_12::Resume()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Resume()" << std::endl;

	if (m_paused) {

		m_paused = false;
	}
}

//-----------------------------------------------------------------------------

void AppState_12::Cleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Cleanup()" << std::endl;

	if (m_cleanup) {

		HandleCleanup();
		m_cleanup = false;
	}
}

} // namespace nxn
