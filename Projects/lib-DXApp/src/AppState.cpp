#include "pch.h"
#include "AppState.h"

#include "Logger.h"
#include "App.h"


namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppState::AppState(App* p_app) :
	m_app(p_app),
	m_initialized(false),
	m_cleanup(false)
{
	GetLogStream(SeverityInfo)
		<< "AppState::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState::~AppState()
{
	GetLogStream(SeverityInfo)
		<< "AppState::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------
/**
 * Initializes the state and make it do some clean-up if needed
 */
void AppState::Init()
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
void AppState::Deinit()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Deinit()" << std::endl;

	if (m_initialized) {

		m_cleanup = true;
		m_initialized = false;
	}
}

//-----------------------------------------------------------------------------

void AppState::Pause()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Pause()" << std::endl;

	if (!m_paused) {

		m_paused = true;
	}
}

//-----------------------------------------------------------------------------

void AppState::Resume()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Resume()" << std::endl;

	if (m_paused) {

		m_paused = false;
	}
}

//-----------------------------------------------------------------------------

void AppState::Cleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppState::Cleanup()" << std::endl;

	if (m_cleanup) {

		HandleCleanup();
		m_cleanup = false;
	}
}

} // namespace nxn
