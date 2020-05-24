#include "AppBase.h"
#include "LoggerBase.h"

AppBase* AppBase::g_instance = NULL;
const LPCWSTR AppBase::g_windowClassName = L"AppBaseWindowClass";

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

std::string AppBase::getName()
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

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	GetLogStream(SeverityInfo)
		<< "AppBase::processArguments() m_name = " << m_name  << ", m_hInstance = " << m_hInstance << std::endl
		<< "m_path = " << m_path << std::endl;

	if (p_argc == 1) {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments() command line : (none)" << std::endl;
	}
	else {
		GetLogStream(SeverityInfo)
			<< "AppBase::processArguments() command line :" << std::endl;
		for (int i = 0; i < p_argc; ++i) {
			GetLogStream(SeverityInfo)
				<< "Argument " << i << " = " << p_argv[i] << std::endl;
		}
	}
}

void AppBase::processArguments(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, PWSTR p_pCmdline, int p_iCmdshow)
{
	TCHAR exepath[MAX_PATH];
	GetModuleFileNameW(0, exepath, MAX_PATH);
	m_path = std::filesystem::path{ exepath };
	m_name = m_path.stem().string();

	// Get the instance of this application.
	m_hInstance = p_hInstance;

	GetLogStream(SeverityInfo)
		<< "AppBase::processArguments()"
		<< "m_name = " << m_name << ", m_hInstance = " << m_hInstance << std::endl
		<< ", m_path = " << m_path << std::endl;
}

int AppBase::run()
{
	GetLogStream(SeverityInfo) << "AppBase::run()" << std::endl;

	m_running = true;

	// TODO ? Will probably need code to initialize things here

	// Do registrations here for the managers and helper classes
	// Do initializations here for the managers ans helper classes
	initWindows();
	init();

	m_updateTimer.init();
	m_frameTimer.init();

	// Make sure we have a current state
	if (m_currentState == nullptr)
	{
		GetLogStream(SeverityFatal)
			<< "AppBase::run() : The application doesnt have a current state" << std::endl;
		throw new std::exception("The application doesnt have a current state");
	}

	m_updateTimer.reset();
	m_frameTimer.reset();

	float updateLag = 0.f;

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
			tick();
		}
	}

	m_exitCode = (int)msg.wParam;

	// Clean up the app
	shutdown();
	shutdownWindows();

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
		<< "AppBase::quit(" << p_exitCode << ")" << std::endl;

	PostQuitMessage(p_exitCode);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return AppBase::getApp()->handleMessage(hwnd, umessage, wparam, lparam);
		}
	}
}

LRESULT CALLBACK AppBase::handleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			// TODO ? send message to current application state
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

void AppBase::initWindows()
{
	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = g_windowClassName;

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	m_windowWidth = GetSystemMetrics(SM_CXSCREEN);
	m_windowHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	int posX, posY;
	if (m_fullScreen)
	{
		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long) m_windowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long) m_windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		m_windowWidth = 800;
		m_windowHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_windowWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_windowHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, s2ws(m_name).c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_windowWidth, m_windowHeight, NULL, NULL, m_hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void AppBase::shutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(g_windowClassName, m_hInstance);
	m_hInstance = NULL;

	return;
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

void AppBase::tick()
{
	/*
	GetLogStream(SeverityInfo)
		<< "AppBase::tick()" << std::endl;
	//*/

	AppStateBase* nextState = nullptr;

	float elapsedUpdate = m_updateTimer.getElapsedTime();
	m_updateTimer.reset();
	/*
	GetLogStream(SeverityInfo)
		<< "AppBase::tick() : elapsedUpdate = " << elapsedUpdate << std::endl;
	//*/

	m_updateLag += elapsedUpdate;

	unsigned int updates = 0;
	while (m_updateLag >= m_updateInterval && updates < m_maxUpdates)
	{
		nextState = m_currentState->updateFixed();
		if (nullptr != nextState) {
			setCurrentState(nextState);
		}
		updates++;
		m_updateLag -= m_updateInterval;

		/*
		GetLogStream(SeverityInfo)
			<< "AppBase::tick() : lag = " << lag << ", updates = " << updates
			<< ", m_updateInterval = " << m_updateInterval << std::endl;
		//*/

	}

	nextState = m_currentState->updateVariable(elapsedUpdate);
	if (nullptr != nextState) {
		setCurrentState(nextState);
	}


	float elapsedFrame = m_frameTimer.getElapsedTime();
	m_frameTimer.reset();
	/*
	GetLogStream(SeverityInfo)
		<< "AppBase::tick() : elapsedFrame = " << elapsedFrame << std::endl;
	//*/

	m_currentState->draw();
}

void AppBase::shutdown()
{
	GetLogStream(SeverityInfo) << "AppBase::shutdown()" << std::endl;

	handleCleanUp();

	// TODO ? Will probably need code to shutdown things here

	// Do de-initializations here for the managers ans helper classes
}