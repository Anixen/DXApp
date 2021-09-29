/**
 * Provides the interface for an application state.
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppState
 * AppState is an implementation of the state pattern (Finished State Machine).
 *
 * @file src/AppState.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <string>
#include "StepTimer.h"
#include "App.h"


namespace nxn {


// Forward declarations
class App;

class AppState {

public:
	                                    ~AppState       ();

	virtual                 void        init            ();
	virtual                 void        reinit          () = 0; // Called to reset the state. Re-Initializes a state without reallocating everything inside.
	                        void        deinit          ();
	        inline  const   bool        isInitialized   () const    { return m_initialized; }

	                        void        pause           ();
	                        void        resume          ();
	        inline  const   bool        isPaused        () const    { return m_paused; }

	/**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
	 *
	 * @return {AppState*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual                 AppState*   update          (DX::StepTimer const& p_timer) = 0;

	virtual                 void        draw            (DX::DeviceResources* p_deviceResources) = 0;
    virtual                 void        clear           (DX::DeviceResources* p_deviceResources) = 0;

	                        void        cleanup         ();

protected:
	                                    AppState        (App* p_app);

    virtual                 void        handleCleanup   () = 0;
	
	                        App*        m_app;  // Pointer to the app the state belongs to

private:
                                        AppState        (const AppState&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            AppState&   operator=       (const AppState&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.

	                        bool        m_initialized;
	                        bool        m_paused;
	                        bool        m_cleanup;

}; // class AppState

} // namespace nxn
