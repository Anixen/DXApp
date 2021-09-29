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

#include "AppState.h"


namespace nxn {


class AppStateTest : public AppState {

public:
	                    AppStateTest    (App* p_app);
	                    ~AppStateTest   ();

	virtual void        init            ();
	virtual void        reinit          ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState*   update          (DX::StepTimer const& p_timer);

	virtual void        draw            (DX::StepTimer const& p_timer, DX::DeviceResources* p_deviceResources);
    virtual void        clear           (DX::DeviceResources* p_deviceResources);

	virtual void        handleCleanup   ();

}; // class AppStateTest

} // namespace nxn
