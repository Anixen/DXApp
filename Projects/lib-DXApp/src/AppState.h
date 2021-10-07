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
 */

#ifndef NXN_APP_STATE
#define NXN_APP_STATE


#include "StepTimer.h"
#include "App.h"

namespace nxn {


// Forward declarations
class App;

class AppState {

    friend class App;
    //friend                void        App::OnDeviceLost                   ();
    //friend                void        App::OnDeviceRestored               ();

public:
	                                    ~AppState                           ();

	virtual                 void        Init                                ();
	virtual                 void        Reinit                              () = 0; // Called to reset the state. Re-Initializes a state without reallocating everything inside.
	                        void        Deinit                              ();
	        inline  const   bool        IsInitialized                       () const    { return m_initialized; }

	                        void        Pause                               ();
	                        void        Resume                              ();
	        inline  const   bool        IsPaused                            () const    { return m_paused; }

	/**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
	 *
	 * @return {AppState*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual                 AppState*   Update                              (   DX::DeviceResources* p_deviceResources,
                                                                                DX::StepTimer const& p_timer, 
                                                                                bool & p_ctrlConnected,
                                                                                DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons) = 0;

	virtual                 void        Draw                                (DX::DeviceResources* p_deviceResources) = 0;
    virtual                 void        Clear                               (DX::DeviceResources* p_deviceResources) = 0;

	                        void        Cleanup                             ();

protected:
	                                    AppState                            (App* p_app);

    virtual                 void        HandleCleanup                       () = 0;
	
	                        App*        m_app;  // Pointer to the app the state belongs to

private:
                                        AppState                            (const AppState&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            AppState&   operator=                           (const AppState&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.

    virtual                 void        CreateDeviceDependentResources      (DX::DeviceResources * p_deviceResources) = 0;
    virtual                 void        CreateWindowSizeDependentResources  (DX::DeviceResources * p_deviceResources) = 0;
    virtual                 void        ResetResources                      () = 0;

	                        bool        m_initialized;
	                        bool        m_paused;
	                        bool        m_cleanup;

}; // class AppState

} // namespace nxn


#endif // !NXN_APP_STATE
