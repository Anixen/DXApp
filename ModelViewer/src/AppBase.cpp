#include "AppBase.h"
#include "LoggerBase.h"


AppBase* AppBase::gInstance = NULL;

AppBase::AppBase() :
	mRunning(false),
	mMaxUpdates(200),
	mUpdateInterval(5),
	mExitCode(0)
{
	LogMessage(SeverityInfo, "AppBase::ctor()")

	gInstance = this;
}

AppBase::~AppBase()
{
	LogMessage(SeverityInfo, "AppBase::dtor()")

	mRunning = false;

	if (this == gInstance) {
		gInstance = NULL;
	}
}


AppBase* AppBase::getApp()
{
	return gInstance;
}

std::string AppBase::getName()
{
	return mName;
}

std::filesystem::path AppBase::getPath()
{
	return mPath;
}

void AppBase::processArguments(int pArgc, char **pArgv)
{
	mPath = std::filesystem::path{ pArgv[0] };
	mName = mPath.stem().string();

	GetLogStream(SeverityInfo)
		<< "AppBase::processArguments(" << pArgv[0] << ")"
		<< ", mPath = " << mPath << ", mName = " << mName << std::endl;

	if (pArgc == 1) {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments(" << pArgv[0] << ") command line : (none)" << std::endl;
	}
	else {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments(" << pArgv[0] << ") command line :" << std::endl;
		for (int i = 0; i < pArgc; ++i) {
			GetLogStream(SeverityInfo)
				<< "Argument " << i << " = " << pArgv[i] << std::endl;
		}
	}
}

int AppBase::run()
{
	GetLogStream(SeverityInfo) << "AppBase::run()" << std::endl;

	mRunning = true;

	// TODO ? Will probably need code to initialize things here

	// Do registrations here for the managers and helper classes
	// Do initializations here for the managers ans helper classes
	init();

	// Start the main loop
	loop();

	// Clean up the app
	cleanup();

	mRunning = false;

	if (mExitCode < 0) {
		GetLogStream(SeverityError)
			<< "AppBase::run() : exitCode = " << mExitCode << std::endl;
	}
	else {
		GetLogStream(SeverityInfo)
			<< "AppBase::run() : exitCode = " << mExitCode << std::endl;
	}

	return mExitCode;
}

bool AppBase::isRunning()
{
	return mRunning;
}

float AppBase::getUpdateInterval()
{
	return mUpdateInterval;
}

void AppBase::setUpdateInterval(float pUpdateInterval)
{
	mUpdateInterval = pUpdateInterval;
}

void AppBase::setMaxUpdates(unsigned int pMaxUpdates)
{
	GetLogStream(SeverityInfo)
		<< "IApp::setMaxUpdates(" << pMaxUpdates << ")" << std::endl;

	if (1 <= pMaxUpdates && pMaxUpdates <= 200)
	{
		mMaxUpdates = pMaxUpdates;
	}
}

void AppBase::quit(int pExitCode)
{
	GetLogStream(SeverityInfo)
		<< "IApp::quit(" << pExitCode << ")" << std::endl;

	mRunning = false;
	mExitCode = pExitCode;
}

void AppBase::setCurrentState(AppStateBase* pState)
{
	GetLogStream(SeverityInfo)
		<< "StateManager::setActiveState()" << std::endl;

	if (nullptr != mCurrentState) {
		mCurrentState->deinit();
		delete mCurrentState;
		mCurrentState = nullptr;
	}

	mCurrentState = pState;
	mCurrentState->init();
}

void AppBase::loop()
{
	GetLogStream(SeverityInfo)
		<< "AppBase::loop()" << std::endl;

	// Make sure we have a current state
	if (mCurrentState == nullptr)
	{
		GetLogStream(SeverityFatal)
			<< "AppBase::loop() : The application doesnt have a current state" << std::endl;
		throw new std::exception("The application doesnt have a current state");
	}
	//*/

	// Clocks to keep track of the time elapsed since last update and last frame
	Timer updateTimer;
	Timer frameTimer;

	updateTimer.init();
	frameTimer.init();

	updateTimer.reset();
	frameTimer.reset();

	float lag = 0.f;
	// Loop while the application is running
	//while(isRunning() && !mStateManager.isEmpty()) {
	while (isRunning())
	{
		// Event loop
		processInput();


		float elapsedUpdate = updateTimer.getElapsedTime();
		updateTimer.reset();
		//*
		GetLogStream(SeverityInfo)
			<< "AppBase::loop() : elapsedUpdate = " << elapsedUpdate << std::endl;
		//*/

		lag += elapsedUpdate;

		unsigned int updates = 0;
		while (lag >= mUpdateInterval && updates < mMaxUpdates)
		{
			mCurrentState->updateFixed();
			updates++;
			lag -= mUpdateInterval;

			/*
			GetLogStream(SeverityInfo)
				<< "AppBase::loop() : lag = " << lag << ", updates = " << updates
				<< ", mUpdateInterval = " << mUpdateInterval << std::endl;
			//*/

		}

		mCurrentState->updateVariable(elapsedUpdate);


		float elapsedFrame = frameTimer.getElapsedTime();
		frameTimer.reset();
		//*
		GetLogStream(SeverityInfo)
			<< "AppBase::loop() : elapsedFrame = " << elapsedFrame << std::endl;
		//*/
		
		mCurrentState->draw();

		/*
		if (WindowManager::isInitialized()) {
			WindowManager::swapBuffers();
		}

		if (WindowManager::shouldClose())
			quit();
		//*/
	}
}

void AppBase::processInput()
{
	/*
	GetLogStream(SeverityDebug)
		<< "IApp::processInput()" << std::endl;
	//*/

	// TODO ? Will probably need code to poll events here
}

void AppBase::cleanup()
{
	GetLogStream(SeverityInfo) << "IApp::cleanup()" << std::endl;

	handleCleanUp();

	// TODO ? Will probably need code to shutdown things here

	// Do de-initializations here for the managers ans helper classes
}