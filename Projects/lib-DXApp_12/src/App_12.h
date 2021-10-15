/**
 * Provides the interface for an application class.
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class App_12
 * App_12 implements the Singleton pattern.
 * App_12 makes use of the state pattern with AppState_12.
 *
 * @file src/App_12.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 * @date 20200529 - Handled Window messages
 */

#ifndef NXN_APP_12
#define NXN_APP_12


#include "DeviceResources_12.h"

#include "AppState_12.h"

namespace nxn {


// Forward declarations
class AppState_12;

class App_12 : public DX::IDeviceNotify {

	friend LRESULT CALLBACK WndProc_12(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	virtual											                ~App_12             ();

	static	inline		    App_12*				                    GetApp				()			                { return g_instance; }
			inline	const	std::string				                GetName				() const	                { return m_name; }
			inline	const	std::filesystem::path	                GetPath				() const	                { return m_path; }

            inline  const   bool                                    IsInitialized       () const                    { return m_initialized; }
			inline	const	bool					                IsRunning           () const                    { return m_running; }

            inline          void                                    SetFixedUpdate      (double p_fixedUpdate)      { m_stepTimer.SetFixedTimeStep(p_fixedUpdate); }
			inline          void					                SetUpdateFrequency  (double p_updateFrequency)  { m_stepTimer.SetTargetElapsedSeconds(1.0 / p_updateFrequency); }

	virtual                 void					                GetDefaultWindowSize(int& width, int& height) const;

	virtual                 void					                ProcessArguments	(int p_argc, char** p_argv);
	virtual                 void					                ProcessArguments	(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, PWSTR p_pCmdline, int p_iCmdshow);

							int						                Run					();
							void					                Quit				(int p_exitCode);

            inline  const   bool                                    IsGamePadConnected  () const                    { return m_gamePadConnected; }

protected:
	                                                                App_12              (std::unique_ptr<DX::DeviceResources_12> & p_deviceResources); // Ctor is protected because we only allow derived classes to instantiate this interface


	virtual                 LRESULT CALLBACK                        HandleMessage       (HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


	                        void                                    InitWindows         ();
	                        void                                    ShutdownWindows     ();

	                        void                                    SetCurrentState     (AppState_12* p_state);
	virtual                 void                                    Tick                ();
    virtual                 void                                    Init                ();
    virtual                 void                                    PreInit             () = 0; // Performs custom steps (e.g. allocating memory) before entering the main loop.
    virtual                 void                                    PostInit            () = 0; // Performs custom steps (e.g. allocating memory) before entering the main loop
	virtual                 void                                    HandleCleanUp       () = 0; // Performs custom steps (e.g. freeing memory) after exiting the main loop

    // IDeviceNotify
    virtual                 void                                    OnDeviceLost        () override;
    virtual                 void                                    OnDeviceRestored    () override;

	virtual                 void                                    OnActivated         ();
	virtual                 void                                    OnDeactivated       ();
	virtual                 void                                    OnSuspending        ();
	virtual                 void                                    OnResuming          ();
	virtual                 void                                    OnWindowSizeChanged (int width, int height);

private:
                                                                    App_12              (const App_12&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            App_12&                                 operator=           (const App_12&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.

                            void                                    Shutdown();
                            
	static                  App_12*                                 g_instance;

	                        std::string                             m_name;
	                        std::filesystem::path                   m_path;

	                        HINSTANCE                               m_hInstance;

	                        HWND                                    m_hwnd;
	                        int                                     m_windowWidth,  m_windowHeight;
	                        int                                     m_windowPosX,   m_windowPosY;

    static          const   LPCWSTR                                 g_windowClassName;
	                        bool                                    m_sizemove          = false;
	                        bool                                    m_suspended         = false;
	                        bool                                    m_minimized         = false;
	                        bool                                    m_fullscreen        = false;

                            bool                                    m_initialized       = false;
	                        bool                                    m_running           = false;
	                        AppState_12*                            m_currentState      = nullptr;

                            // Device resources.
                            std::unique_ptr<DX::DeviceResources_12> m_deviceResources;

                            // Rendering loop timer.
	                        DX::StepTimer                           m_stepTimer;

	                        int                                     m_exitCode          = 0;

                            // Input devices.
                            std::unique_ptr<DirectX::GamePad>       m_gamePad;
                            std::unique_ptr<DirectX::Keyboard>      m_keyboard;
                            std::unique_ptr<DirectX::Mouse>         m_mouse;

                            bool                                    m_gamePadConnected  = false;
                            DirectX::GamePad::ButtonStateTracker    m_gamePadButtons;
                            DirectX::Keyboard::KeyboardStateTracker m_keyboardButtons;
                            DirectX::Mouse::ButtonStateTracker      m_mouseButtons;

}; // class App_12

} // namespace nxn


#endif // !NXN_APP_12
