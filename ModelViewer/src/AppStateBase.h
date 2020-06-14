/**
 * Provides the interface for an application state.
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateBase
 * AppStateBase is an implementation of the state pattern (Finished State Machine).
 *
 * @file src/AppStateBase.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 */

#pragma once

#include <string>
#include "Timer.h"
#include "AppBase.h"

// Forward declarations
class AppBase;

class AppStateBase {

public:

	/**
	 * AppStateBase de-constructor
	 */
	~AppStateBase();


	/**
	 * Initializes the state and make it do some clean-up if needed
	 */
	virtual void init();

	/**
	 * Re-Initializes a state without reallocating everything inside.
	 * This function should be called whenever a state is reset.
	 */
	virtual void reinit() = 0;

	/**
	 * Un-Initializes a state and marks it for clean-up
	 * This function should be called whenever a state becomes inactive.
	 */
	void deinit();

	/**
	 *
	 */
	bool isInitialized() const;


	/**
	 *
	 */
	void pause();

	/**
	 *
	 */
	void resume();

	/**
	 *
	 */
	bool isPaused() const;

	/**
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual AppStateBase* updateFixed() = 0;

	/**
	 * @param {float} p_elapsedTime The duration in ms since last update
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual AppStateBase* updateVariable(float p_elapsedTime) = 0;

	/**
	 *
	 */
	virtual void draw() = 0;


	/**
	 *
	 */
	void cleanup();

	/**
	 *
	 */
	float getElapsedTime() const;


protected:
	/**
	 * AppStateBase constructor
	 * Is protected because we only allow derived classes to instantiate this interface
	 *
	 * @param {AppBase*} p_app The address of the app to which pertains this state
	 */
	AppStateBase(AppBase* p_app);

	// Address of the app to which pertains this state
	AppBase* m_app;

	/**
	 *
	 */
	virtual void handleCleanup() = 0;

private:

	bool m_initialized;
	bool m_paused;
	bool m_cleanup;

	float m_elapsedTime;
	float m_totalPausedTime;

	Timer m_elapsedClock;
	Timer m_pausedClock;

	/**
	 * AppStateBase assignment operator
	 * is private because we do not allow copying items of this class
	 */
	AppStateBase(const AppStateBase&);               // Intentionally undefined

	/**
	 * AppStateBase assignment operator
	 * is private because we do not allow copying items of this class
	 */
	AppStateBase& operator=(const AppStateBase&);    // Intentionally undefined

}; // class AppStateBase
