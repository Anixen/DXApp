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
	/**
	 * AppBase deconstructor
	 */
	virtual ~AppBase();


	/**
	 *  @return {AppBase*} a pointer to the most recently instantiated application
	 */
	static AppBase* getApp();

	/**
	 * @return {std::string} the name of the application
	 */
	std::string getName();

	/**
	 * @return {std::string} the name of the application
	 */
	std::filesystem::path getPath();

	/**
	 *
	 */
	virtual void getDefaultWindowSize(int& width, int& height) const;

	/**
	 * handles command line arguments
	 *
	 * @param {int} p_argc The number of arguments
	 * @param {char**} p_argv The actual arguments
	 */
	virtual void processArguments(int p_argc, char** p_argv);

	/**
	 * handles Win32 arguments
	 *
	 * @param {HINSTANCE}	p_hInstance		A value used by the system to identify the exececutable instance
	 * @param {HINSTANCE}	p_hPrevInstance	Has no meaning in Win32, should always be 0
	 * @param {PWSTR}		p_pCmdline		The command line as a unicode string
	 * @param {int}			p_iCmdshow		A flag to indicate if the application window should be minimized, maximized or shown normally
	 */
	virtual void processArguments(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, PWSTR p_pCmdline, int p_iCmdshow);


	/**
	 * Initializes and starts the application main loop,
	 * it will also handle cleaning up once the main loop has ended
	 *
	 * This functions relies on the loop, init and shutdown methods,
	 * which are defined by derived classes
	 */
	int run();

	/**
	 *  @return {bool} true if the application is running, false if not
	 */
	bool isRunning();

	/**
	 * @return {float} the update interval in ms of the application
	 */
	float getUpdateInterval();

	/**
	 * Sets the update interval of the application
	 *
	 * @param {float} p_updateInterval The new update interval in ms for the application
	 */
	void setUpdateInterval(float p_updateInterval);


	/**
	 *
	 */
	void quit(int p_exitCode);

protected:

	/**
	 * AppBase constructor
	 * Is protected because we only allow derived classes to instantiate this interface
	 */
	AppBase();

	/**
	 * Handles windows messages
	 */
	virtual LRESULT CALLBACK handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	/**
	 * Initializes the application window
	 */
	void initWindows();

	/**
	 * Closes the application window
	 */
	void shutdownWindows();

	/**
	 * Initializes the application components
	 * is called before the main loop
	 */
	virtual void init() = 0;

	/**
	 * Sets the current app state
	 *
	 * @param {AppStateBase} p_state The new state for the application
	 */
	void setCurrentState(AppStateBase* p_state);

	/**
	 * Advances the application one tick forward,
	 * which involves updating its components, and rendering a frame
	 */
	virtual void tick();

	/**
	 * Performs some custom clean before exiting the application
	 */
	virtual void handleCleanUp() = 0;


	/**
	 *
	 */
	virtual void onActivated();

	/**
	 *
	 */
	virtual void onDeactivated();

	/**
	 *
	 */
	virtual void onSuspending();

	/**
	 *
	 */
	virtual void onResuming();

	/**
	 *
	 */
	virtual void onWindowSizeChanged();

private:

	static AppBase* g_instance;

	std::string m_name;
	std::filesystem::path m_path;

	HINSTANCE m_hInstance;

	HWND m_hwnd;
	int m_windowWidth, m_windowHeight;
	int m_windowPosX, m_windowPosY;
	static const LPCWSTR g_windowClassName;
	bool m_sizemove = false;
	bool m_suspended = false;
	bool m_minimized = false;
	bool m_fullscreen = false;

	bool m_running;
	AppStateBase* m_currentState;

	StepTimer m_stepTimer;
	uint64_t m_updateInterval;

	int m_exitCode;

	/**
	 *
	 */
	void shutdown();

	/**
	 * AppBase assignment operator
	 * is private because we do not allow copies of a Singleton
	 */
	AppBase(const AppBase&);               // Intentionally undefined

	/**
	 * AppBase assignment operator
	 * is private because we do not allow copies of a Singleton
	 */
	AppBase& operator=(const AppBase&);    // Intentionally undefined

}; // class AppBase