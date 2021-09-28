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
#include "Timer.h"
#include "App.h"


namespace nxn {


// Forward declarations
class App;

class AppState {

public:
	                                    ~AppState();

	virtual                 void        init            ();
	virtual                 void        reinit          () = 0; // Called to reset the state. Re-Initializes a state without reallocating everything inside.
	                        void        deinit          ();
	        inline  const   bool        isInitialized   () const    { return m_initialized; }

	                        void        pause           ();
	                        void        resume          ();
	        inline  const   bool        isPaused        () const    { return m_paused; }

	/**
	 * @param {double}  p_elapsedTime   The duration in seconds since last update
	 *
	 * @return {AppState*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual                 AppState*   update          (double p_elapsedSeconds) = 0;

	virtual                 void        draw            () = 0;
	                        void        cleanup         ();

	                        float       getElapsedTime  () const;

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

	                        float       m_elapsedTime;
	                        float       m_totalPausedTime;

	                        Timer       m_elapsedClock;
	                        Timer       m_pausedClock;

}; // class AppState

} // namespace nxn
