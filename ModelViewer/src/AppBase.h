/**
 * Provides the interface for an application class.
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppBase
 * AppBase implements the Singleton pattern.
 * AppBase makes use of the state pattern with AppStateBase.
 *
 * @file src/AppBase.h
 * @author Olivier Falconnet
 * @date 20200522 - File creation
 * @date 20200529 - Handled Window messages
 * @date 20210927 - Updated coding style
 */

#pragma once

#include <string>
#include <filesystem>
#include "AppStateBase.h"
#include "StepTimer.h"


// Forward declarations
class AppStateBase;

class AppBase {

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	virtual											~AppBase            ();

	static	inline		    AppBase*				getApp				()			                { return g_instance; }
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
	                                                AppBase             (); // Ctor is protected because we only allow derived classes to instantiate this interface


	virtual                 LRESULT CALLBACK        handleMessage       (HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


	                        void                    initWindows         ();
	                        void                    shutdownWindows     ();

	                        void                    setCurrentState     (AppStateBase* p_state);
	virtual                 void                    tick                ();
    virtual                 void                    init                () = 0; // Performs custom steps (e.g. allocating memory) before entering the main loop
	virtual                 void                    handleCleanUp       () = 0; // Performs custom steps (e.g. freeing memory) after exiting the main loop


	virtual                 void                    onActivated         ();
	virtual                 void                    onDeactivated       ();
	virtual                 void                    onSuspending        ();
	virtual                 void                    onResuming          ();
	virtual                 void                    onWindowSizeChanged ();

private:
                                                    AppBase             (const AppBase&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.
                            AppBase&                operator=           (const AppBase&);   // Intentionally undefined. Is private because we do not allow copies of a Singleton.

                            void                    shutdown();
                            
	static                  AppBase*                g_instance;

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

	                        bool                    m_running;
	                        AppStateBase*           m_currentState;

	                        StepTimer               m_stepTimer;
	                        uint64_t                m_updateInterval;

	                        int                     m_exitCode;

}; // class AppBase
