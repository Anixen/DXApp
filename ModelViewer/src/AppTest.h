/**
 * Testing implementation application class.
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppTest
 *
 *
 * @file src/AppTest.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 */

#pragma once

#include "AppBase.h"


class AppTest : public AppBase {

public:

	/**
	 * TestApp constructor
	 */
	AppTest();

	/**
	 * TestApp de-constructor
	 */
	~AppTest();

protected:

	/**
	 * initializes the application components such as the WindowManager or the StateManager
	 * is called before the main loop
	 */
	virtual void init();

	/**
	 * performs some custom clean before exiting the application
	 * is called before the main loop
	 */
	virtual void handleCleanUp();

private:

};