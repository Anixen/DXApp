/**
 * @file Provides the interface for an application class.
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * AppBase implements the Singleton pattern.
 * AppBase makes use of the state pattern with AppStateBase.
 *
 * @author Olivier Falconnet
 */

#pragma once

#include <string>
#include <filesystem>
#include "AppStateBase.h"

// Forward declarations
class AppStateBase;

class AppBase {

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
	 * @return {LPCWSTR} the name of the application
	 */
	LPCWSTR getName();

	/**
	 * @return {std::string} the name of the application
	 */
	std::filesystem::path getPath();

	/**
	 * handles command line arguments
	 *
	 * @param {int} p_argc The number of arguments
	 * @param {char**} p_argv The actual arguments
	 */
	virtual void processArguments(int p_argc, char** p_argv);


	/**
	 * Initializes and starts the application main loop,
	 * it will also handle cleaning up once the main loop has ended
	 *
	 * This functions relies on the loop, init and cleanup methods,
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
	 * Sets the maximum consecutive updates for the application
	 *
	 * @param {float} p_maxUpdates The new maximum number of consecutive updates in Hz for the application
	 */
	void setMaxUpdates(unsigned int p_maxUpdates);


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
	 * Contains the logic for the main loop of the application,
	 * which involves updating its components,
	 * handling input from the user, and giving him output (display)
	 */
	virtual void loop();

	/**
	 * Handles input from the user
	 *
	 * @return {AppStateBase*} A pointer to a new app state, or nullptr if no change required
	 */
	virtual AppStateBase* processInput();

	/**
	 * Performs some custom clean before exiting the application
	 * is called before the main loop
	 */
	virtual void handleCleanUp() = 0;

private:

	static AppBase* g_instance;

	LPCWSTR m_name;
	std::filesystem::path m_path;

	bool m_running;
	AppStateBase* m_currentState;

	float m_updateInterval;
	unsigned int m_maxUpdates;

	int m_exitCode;

	/**
	 *
	 */
	void cleanup();

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