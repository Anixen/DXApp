/**
 * Provides the interface for an application state.
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppState_11
 * AppState_11 is an implementation of the state pattern (Finished State Machine).
 *
 * @file src/AppState_11.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 */

#ifndef NXN_APP_STATE_11
#define NXN_APP_STATE_11


#include "App_11.h"

namespace nxn {


// Forward declarations
class App_11;

class AppState_11 {

    friend                  class           App_11;
    //friend                void            App_11::OnDeviceLost                ();
    //friend                void            App_11::OnDeviceRestored            ();

public:
	                                        ~AppState_11                        ();

	virtual                 void            Init                                ();
	virtual                 void            Reinit                              () = 0; // Called to reset the state. Re-Initializes a state without reallocating everything inside.
	                        void            Deinit                              ();
	        inline  const   bool            IsInitialized                       () const    { return m_initialized; }

	                        void            Pause                               ();
	                        void            Resume                              ();
	        inline  const   bool            IsPaused                            () const    { return m_paused; }

	/**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
	 *
	 * @return {AppState_11*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual                 AppState_11*    Update                              (   DX::DeviceResources_11* p_deviceResources,
                                                                                    DX::StepTimer const& p_timer, 
                                                                                    bool & p_gamePadConnected,
                                                                                    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons) = 0;

	virtual                 void            Draw                                (DX::DeviceResources_11* p_deviceResources) = 0;
    virtual                 void            Clear                               (DX::DeviceResources_11* p_deviceResources) = 0;

	                        void            Cleanup                             ();

protected:
	                                        AppState_11                         (App_11* p_app);

    virtual                 void            HandleCleanup                       () = 0;
	
	                        App_11*         m_app;  // Pointer to the app the state belongs to

private:
                                            AppState_11                         (const AppState_11&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            AppState_11&    operator=                           (const AppState_11&);  // Intentionally undefined. Is private because we do not allow copies of a Singleton.

    virtual                 void            CreateDeviceDependentResources      (DX::DeviceResources_11 * p_deviceResources) = 0;
    virtual                 void            CreateWindowSizeDependentResources  (DX::DeviceResources_11 * p_deviceResources) = 0;
    virtual                 void            ResetResources                      () = 0;

	                        bool            m_initialized;
	                        bool            m_paused;
	                        bool            m_cleanup;

}; // class AppState_11

} // namespace nxn


#endif // !NXN_APP_STATE_11
