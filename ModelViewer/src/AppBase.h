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
	 * @return {std::string} the name of the application
	 */
	std::string getName();

	/**
	 * @return {std::string} the name of the application
	 */
	std::filesystem::path getPath();

	/**
	 * handles command line arguments
	 *
	 * @param {int} pArgc The number of arguments
	 * @param {char**} pArgv The actual arguments
	 */
	virtual void processArguments(int pArgc, char** pArgv);


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
	 * @return {float} the update rate in Hz of the application
	 */
	float getUpdateRate();

	/**
	 * Sets the update rate in Hz for the application
	 *
	 * @param {float} pUpdateRate The new update rate in Hz for the application
	 */
	void setUpdateRate(float pUpdateRate);

	/**
	 * Sets the maximum consecutive updates for the application
	 *
	 * @param {float} pMaxUpdates The new maximum number of consecutive updates in Hz for the application
	 */
	void setMaxUpdates(unsigned int pMaxUpdates);


	/**
	 *
	 */
	void quit(int pExitCode);

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
	 * Contains the logic for the main loop of the application,
	 * which involves updating its components,
	 * handling input from the user, and giving him output (display)
	 */
	virtual void loop();

	/**
	 * Handles input from the user through GLFW
	 */
	virtual void processInput();

	/**
	 * Performs some custom clean before exiting the application
	 * is called before the main loop
	 */
	virtual void handleCleanUp() = 0;

	AppStateBase* mCurrentState;

private:

	static AppBase* gInstance;

	std::string mName;
	std::filesystem::path mPath;

	bool mRunning;

	float mUpdateRate;
	float mUpdateInterval;
	unsigned int mMaxUpdates;

	int mExitCode;

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