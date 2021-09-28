/**
 * Provides the interface for an application class.
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class App
 * App implements the Singleton pattern.
 * App makes use of the state pattern with AppState.
 *
 * @file src/App.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 * @date 20200529 - Handled Window messages
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <string>
#include <filesystem>
#include "AppState.h"
#include "StepTimer.h"


namespace nxn {


// Forward declarations
class AppState;

class App {

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	virtual											~App                ();

	static	inline		    App*				    getApp				()			                { return g_instance; }
			inline	const	std::string				getName				() const	                { return m_name; }
			inline	const	std::filesystem::path	getPath				() const	                { return m_path; }

			inline	const	bool					isRunning           () const                    { return m_running; }
			inline	const	float					getUpdateInterval   () const                    { return m_updateInterval; }
			inline			void					setUpdateInterval   (float p_updateInterval)    { m_updateInterval = p_updateInterval; }

	virtual                 void					getDefaultWindowSize(int& width, int& height) const;

	virtual                 void					processArguments	(int p_argc, char** p_argv);
	virtual                 void					processArguments	(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, PWSTR p_pCmdline, int p_iCmdshow);

							int						run					();
							void					quit				(int p_exitCode);

protected:
	                                                App                 (); // Ctor is protected because we only allow derived classes to instantiate this interface


	virtual                 LRESULT CALLBACK        handleMessage       (HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


	                        void                    initWindows         ();
	                        void                    shutdownWindows     ();

	                        void                    setCurrentState     (AppState* p_state);
	virtual                 void                    tick                ();
    virtual                 void                    init                () = 0; // Performs custom steps (e.g. allocating memory) before entering the main loop
	virtual                 void                    handleCleanUp       () = 0; // Performs custom steps (e.g. freeing memory) after exiting the main loop


	virtual                 void                    onActivated         ();
	virtual                 void                    onDeactivated       ();
	virtual                 void                    onSuspending        ();
	virtual                 void                    onResuming          ();
	virtual                 void                    onWindowSizeChanged ();

private:
                                                    App                 (const App&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            App&                    operator=           (const App&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.

                            void                    shutdown();
                            
	static                  App*                    g_instance;

	                        std::string             m_name;
	                        std::filesystem::path   m_path;

	                        HINSTANCE               m_hInstance;

	                        HWND                    m_hwnd;
	                        int                     m_windowWidth,  m_windowHeight;
	                        int                     m_windowPosX,   m_windowPosY;

    static          const   LPCWSTR                 g_windowClassName;
	                        bool                    m_sizemove      = false;
	                        bool                    m_suspended     = false;
	                        bool                    m_minimized     = false;
	                        bool                    m_fullscreen    = false;

	                        bool                    m_running           = false;
	                        AppState*               m_currentState      = NULL;

	                        StepTimer               m_stepTimer;
	                        float                   m_updateInterval    = 0.005f;

	                        int                     m_exitCode          = 0;

}; // class App

} // namespace nxn
