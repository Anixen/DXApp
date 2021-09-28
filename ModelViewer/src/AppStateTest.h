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
 * @date 20210927 - Updated coding style
 */

#pragma once

#include "AppStateBase.h"


namespace nxn {


class AppStateTest : public AppStateBase {

public:
	                        AppStateTest    (AppBase* p_app);
	                        ~AppStateTest   ();

	virtual void            init            ();
	virtual void            reinit          ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {double}  p_elapsedTime   The duration in seconds since last update
     *
     * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppStateBase*   update          (double p_elapsedSeconds);

	virtual void            draw            ();
	virtual void            handleCleanup   ();

}; // class AppStateTest

} // namespace nxn
