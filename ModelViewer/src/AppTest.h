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
 * @date 20210927 - Updated coding style
 */

#pragma once

#include "App.h"


namespace nxn {


class AppTest : public App {

public:
	                AppTest         ();
	                ~AppTest        ();

protected:
	virtual void    init            (); // Performs custom steps (e.g. allocating memory) before entering the main loop
	virtual void    handleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

}; // class AppTest

} // namespace nxn
