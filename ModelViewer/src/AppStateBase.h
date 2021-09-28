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
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <string>
#include "Timer.h"
#include "AppBase.h"


namespace nxn {


// Forward declarations
class AppBase;

class AppStateBase {

public:
	                                        ~AppStateBase();


	virtual                 void            init            ();
	virtual                 void            reinit          () = 0; // Called to reset the state. Re-Initializes a state without reallocating everything inside.
	                        void            deinit          ();
	        inline  const   bool            isInitialized   () const    { return m_initialized; }

	                        void            pause           ();
	                        void            resume          ();
	        inline  const   bool            isPaused        () const    { return m_paused; }

	/**
	 * @param {double}  p_elapsedTime   The duration in seconds since last update
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual                 AppStateBase*   update          (double p_elapsedSeconds) = 0;

	virtual                 void            draw            () = 0;
	                        void            cleanup         ();

	                        float           getElapsedTime  () const;

protected:
	                                        AppStateBase    (AppBase* p_app);

    virtual                 void            handleCleanup   () = 0;
	
	                        AppBase*        m_app;  // Pointer to the app the state belongs to

private:
                                            AppStateBase    (const AppStateBase&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            AppStateBase&   operator=       (const AppStateBase&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.

	                        bool            m_initialized;
	                        bool            m_paused;
	                        bool            m_cleanup;

	                        float           m_elapsedTime;
	                        float           m_totalPausedTime;

	                        Timer           m_elapsedClock;
	                        Timer           m_pausedClock;

}; // class AppStateBase

} // namespace nxn
