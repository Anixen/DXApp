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
	                    AppStateTest                        (App* p_app);
	                    ~AppStateTest                       ();

	virtual void        init                                ();
	virtual void        reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState*   update                              (   DX::StepTimer const& p_timer,
                                                                DirectX::GamePad* p_gamePad, DirectX::Keyboard* p_keyboard, DirectX::Mouse* p_mouse);

	virtual void        draw                                (DX::DeviceResources* p_deviceResources);
    virtual void        clear                               (DX::DeviceResources* p_deviceResources);

protected:
	virtual void        handleCleanup                       ();

private:
    virtual void        createDeviceDependentResources      (DX::DeviceResources * p_deviceResources);
    virtual void        createWindowSizeDependentResources  ();
    virtual void        resetResources                      ();

}; // class AppStateTest

} // namespace nxn
