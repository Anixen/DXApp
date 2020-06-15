/**
 * Test implementation for an application state.
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateTest
 *
 *
 * @file src/AppStateTest.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 */

#pragma once

#include "AppStateBase.h"

class AppStateTest : public AppStateBase {

public:
	/**
	 * TestState constructor
	 */
	AppStateTest(AppBase* p_app);

	/**
	 *
	 */
	~AppStateTest();

	/**
	 * Initializes the state and make it do some clean-up if needed
	 */
	virtual void init();

	/**
	 * Re-Initializes a state without reallocating everything inside.
	 * This function should be called whenever a state is reset.
	 */
	virtual void reinit();

	/**
	 * @param {double} p_elapsedSeconds The duration in seconds since last update
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual AppStateBase* update(double p_elapsedSeconds);

	/**
	 *
	 */
	virtual void draw();

	/**
	 *
	 */
	virtual void handleCleanup();

private:
};