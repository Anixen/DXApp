#include "AppBase.h"
#include "LoggerBase.h"


AppBase* AppBase::g_instance = NULL;

AppBase::AppBase() :
	m_running(false),
	m_maxUpdates(200),
	m_updateInterval(5),
	m_exitCode(0)
{
	LogMessage(SeverityInfo, "AppBase::ctor()")

	g_instance = this;
}

AppBase::~AppBase()
{
	LogMessage(SeverityInfo, "AppBase::dtor()")

	m_running = false;

	if (this == g_instance) {
		g_instance = NULL;
	}
}


AppBase* AppBase::getApp()
{
	return g_instance;
}

LPCWSTR AppBase::getName()
{
	return m_name;
}

std::filesystem::path AppBase::getPath()
{
	return m_path;
}

void AppBase::processArguments(int p_argc, char **p_argv)
{
	m_path = std::filesystem::path{ p_argv[0] };
	m_name = m_path.stem().string();

	GetLogStream(SeverityInfo)
		<< "AppBase::processArguments(" << p_argv[0] << ")"
		<< ", mPath = " << m_path << ", mName = " << m_name << std::endl;

	if (p_argc == 1) {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments(" << p_argv[0] << ") command line : (none)" << std::endl;
	}
	else {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments(" << p_argv[0] << ") command line :" << std::endl;
		for (int i = 0; i < p_argc; ++i) {
			GetLogStream(SeverityInfo)
				<< "Argument " << i << " = " << p_argv[i] << std::endl;
		}
	}
}

int AppBase::run()
{
	GetLogStream(SeverityInfo) << "AppBase::run()" << std::endl;

	m_running = true;

	// TODO ? Will probably need code to initialize things here

	// Do registrations here for the managers and helper classes
	// Do initializations here for the managers ans helper classes
	init();

	// Start the main loop
	loop();

	// Clean up the app
	cleanup();

	m_running = false;

	if (m_exitCode < 0) {
		GetLogStream(SeverityError)
			<< "AppBase::run() : exitCode = " << m_exitCode << std::endl;
	}
	else {
		GetLogStream(SeverityInfo)
			<< "AppBase::run() : exitCode = " << m_exitCode << std::endl;
	}

	return m_exitCode;
}

bool AppBase::isRunning()
{
	return m_running;
}

float AppBase::getUpdateInterval()
{
	return m_updateInterval;
}

void AppBase::setUpdateInterval(float p_updateInterval)
{
	m_updateInterval = p_updateInterval;
}

void AppBase::setMaxUpdates(unsigned int p_maxUpdates)
{
	GetLogStream(SeverityInfo)
		<< "IApp::setMaxUpdates(" << p_maxUpdates << ")" << std::endl;

	if (1 <= p_maxUpdates && p_maxUpdates <= 200)
	{
		m_maxUpdates = p_maxUpdates;
	}
}

void AppBase::quit(int p_exitCode)
{
	GetLogStream(SeverityInfo)
		<< "IApp::quit(" << p_exitCode << ")" << std::endl;

	PostQuitMessage(p_exitCode);
	m_exitCode = p_exitCode;
}

void AppBase::setCurrentState(AppStateBase* p_state)
{
	GetLogStream(SeverityInfo)
		<< "StateManager::setCurrentState()" << std::endl;

	if (nullptr != m_currentState) {
		m_currentState->deinit();
		delete m_currentState;
		m_currentState = nullptr;
	}

	m_currentState = p_state;
	m_currentState->init();
}

void AppBase::loop()
{
	GetLogStream(SeverityInfo)
		<< "AppBase::loop()" << std::endl;

	// Make sure we have a current state
	if (m_currentState == nullptr)
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

	float updateLag = 0.f;
	AppStateBase* nextState = nullptr;

	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Event loop
			nextState = processInput();
			if (nullptr != nextState) {
				setCurrentState(nextState);
			}

			float elapsedUpdate = updateTimer.getElapsedTime();
			updateTimer.reset();
			//*
			GetLogStream(SeverityInfo)
				<< "AppBase::loop() : elapsedUpdate = " << elapsedUpdate << std::endl;
			//*/

			updateLag += elapsedUpdate;

			unsigned int updates = 0;
			while (updateLag >= m_updateInterval && updates < m_maxUpdates)
			{
				nextState = m_currentState->updateFixed();
				if (nullptr != nextState) {
					setCurrentState(nextState);
				}
				updates++;
				updateLag -= m_updateInterval;

				/*
				GetLogStream(SeverityInfo)
					<< "AppBase::loop() : lag = " << lag << ", updates = " << updates
					<< ", m_updateInterval = " << m_updateInterval << std::endl;
				//*/

			}

			nextState = m_currentState->updateVariable(elapsedUpdate);
			if (nullptr != nextState) {
				setCurrentState(nextState);
			}


			float elapsedFrame = frameTimer.getElapsedTime();
			frameTimer.reset();
			//*
			GetLogStream(SeverityInfo)
				<< "AppBase::loop() : elapsedFrame = " << elapsedFrame << std::endl;
			//*/

			m_currentState->draw();
		}
	}

}

AppStateBase* AppBase::processInput()
{
	/*
	GetLogStream(SeverityDebug)
		<< "IApp::processInput()" << std::endl;
	//*/

	// TODO ? Will probably need code to poll events here

	return nullptr;
}

void AppBase::cleanup()
{
	GetLogStream(SeverityInfo) << "IApp::cleanup()" << std::endl;

	handleCleanUp();

	// TODO ? Will probably need code to shutdown things here

	// Do de-initializations here for the managers ans helper classes
}