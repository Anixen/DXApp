/**
 * @file Provides the interface for an application state.
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * AppStateBase is an implementation of the state pattern (Finished State Machine).
 *
 * @author Olivier Falconnet
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
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual AppStateBase* updateVariable(float elapsedTime) = 0;

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
	 * @param {AppBase*} pApp The address of the app to which pertains this state
	 */
	AppStateBase(AppBase* pApp);

	// Address of the app to which pertains this state
	AppBase* mApp;

	/**
	 *
	 */
	virtual void handleCleanup() = 0;

private:

	bool mInitialized;
	bool mPaused;
	bool mCleanup;

	float mElapsedTime;
	float mTotalPausedTime;

	Timer mElapsedClock;
	Timer mPausedClock;

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
