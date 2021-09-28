#include "App.h"
#include "Logger.h"


namespace nxn {


App* App::g_instance = NULL;
const LPCWSTR App::g_windowClassName = L"AppBaseWindowClass";

//-----------------------------------------------------------------------------

App::App()
{
	LogMessage(SeverityInfo, "AppBase::ctor()")

	g_instance = this;

    m_stepTimer.SetFixedTimeStep(true);
    m_stepTimer.SetTargetElapsedTicks((uint64_t)(m_updateInterval * StepTimer::TicksPerSecond));
}

//-----------------------------------------------------------------------------

App::~App()
{
	LogMessage(SeverityInfo, "AppBase::dtor()")

	m_running = false;

	if (this == g_instance) {
		g_instance = NULL;
	}
}

//-----------------------------------------------------------------------------

void App::setUpdateInterval(float p_updateInterval)
{
    m_updateInterval = p_updateInterval;
    m_stepTimer.SetTargetElapsedTicks((uint64_t)(m_updateInterval * StepTimer::TicksPerSecond));
}

//-----------------------------------------------------------------------------

void App::getDefaultWindowSize(int& width, int& height) const
{
	width  = 800;
	height = 600;
}

//-----------------------------------------------------------------------------
/**
 * handles command line arguments
 *
 * @param {int}			p_argc	The number of arguments
 * @param {char**}		p_argv	The actual arguments
 */
void App::processArguments(int p_argc, char **p_argv)
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

//-----------------------------------------------------------------------------
/**
 * handles Win32 arguments
 *
 * @param {HINSTANCE}	p_hInstance		A value used by the system to identify the exececutable instance
 * @param {HINSTANCE}	p_hPrevInstance	Has no meaning in Win32, should always be 0
 * @param {PWSTR}		p_pCmdline		The command line as a unicode string
 * @param {int}			p_iCmdshow		A flag to indicate if the application window should be minimized, maximized or shown normally
 */
void App::processArguments(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, PWSTR p_pCmdline, int p_iCmdshow)
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

//-----------------------------------------------------------------------------
/**
 * Initializes and starts the application main loop,
 * it will also handle cleaning up once the main loop has ended
 *
 * This functions relies on the loop, init and shutdown methods,
 * which are defined by derived classes
 */
int App::run()
{
	GetLogStream(SeverityInfo) << "AppBase::run()" << std::endl;

	m_running = true;

	// TODO ? Will probably need code to initialize things here

	// Do registrations here for the managers and helper classes
	// Do initializations here for the managers ans helper classes
	initWindows();
	init();

	// Make sure we have a current state
	if (m_currentState == nullptr)
	{
		GetLogStream(SeverityFatal)
			<< "AppBase::run() : The application doesnt have a current state" << std::endl;
		throw new std::exception("The application doesnt have a current state");
	}

	m_stepTimer.ResetElapsedTime();

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

//-----------------------------------------------------------------------------
/**
 *
 */
void App::quit(int p_exitCode)
{
	GetLogStream(SeverityInfo)
		<< "AppBase::quit(" << p_exitCode << ")" << std::endl;

	PostQuitMessage(p_exitCode);
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		// Check if the window is being closed.
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			//Mouse::ProcessMessage(umessage, wparam, lparam);
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			//Keyboard::ProcessMessage(umessage, wparam, lparam);
			break;

		case WM_GETMINMAXINFO:
			{
				auto info = reinterpret_cast<MINMAXINFO*>(lparam);
				info->ptMinTrackSize.x = 320;
				info->ptMinTrackSize.y = 200;
			}
			break;

		case WM_MENUCHAR:
			// A menu is active and the user presses a key that does not correspond
			// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
			return MAKELRESULT(0, MNC_CLOSE);

		default:
			break;
	}

	// All other messages pass to the message handler in the system class.
	return App::getApp()->handleMessage(hwnd, umessage, wparam, lparam);
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK App::handleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (umessage)
	{
	case WM_PAINT:
		if (m_sizemove)
		{
			tick();
		}
		else
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		break;

	case WM_ACTIVATEAPP:
		//Keyboard::ProcessMessage(umessage, wparam, lparam);
		//Mouse::ProcessMessage(umessage, wparam, lparam);

		if (wparam)
		{
			onActivated();
		}
		else
		{
			onDeactivated();
		}
		break;

	case WM_POWERBROADCAST:
		switch (wparam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!m_suspended)
				onSuspending();
			m_suspended = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!m_minimized)
			{
				if (m_suspended)
					onResuming();
				m_suspended = false;
			}
			return TRUE;
		}
		break;

	case WM_SIZE:
		if (wparam == SIZE_MINIMIZED)
		{
			if (!m_minimized)
			{
				m_minimized = true;
				if (!m_suspended)
					onSuspending();
				m_suspended = true;
			}
		}
		else if (m_minimized)
		{
			m_minimized = false;
			if (m_suspended)
				onResuming();
			m_suspended = false;
		}
		else if (!m_sizemove)
		{
			m_windowWidth = LOWORD(lparam);
			m_windowHeight = HIWORD(lparam);
			onWindowSizeChanged();
		}
		break;

	case WM_ENTERSIZEMOVE:
		m_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		m_sizemove = false;

		RECT rc;
		GetWindowRect(m_hwnd, &rc);

		m_windowPosX = rc.left;
		m_windowPosY = rc.top;
		m_windowWidth  = rc.right - rc.left;
		m_windowHeight = rc.bottom - rc.top;
		onWindowSizeChanged();

		break;

	case WM_SYSKEYDOWN:
		if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (m_fullscreen)
			{
				SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX);
				SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, 0);

				ShowWindow(m_hwnd, SW_SHOWNORMAL);

				SetWindowPos(m_hwnd, HWND_TOP, m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, 
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(m_hwnd, GWL_STYLE, 0);
				SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
			}

			m_fullscreen = !m_fullscreen;
		}
		break;

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
			// TODO ? send message to current application state
			// return DefWindowProc(hwnd, umessage, wparam, lparam); // move into app state message handler
			break;
	}

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

//-----------------------------------------------------------------------------

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

void App::initWindows()
{
	// Setup the windows class with default settings
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIconW(m_hInstance, L"IDI_ICON");
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = g_windowClassName;

	// Register the window class
	if (!RegisterClassExW(&wcex))
		throw new std::exception("Failed to register the window class.");

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.

	// Place the window in the middle of the screen.
	getDefaultWindowSize(m_windowWidth, m_windowHeight);
	m_windowPosX = (GetSystemMetrics(SM_CXSCREEN) - m_windowWidth)  / 2;
	m_windowPosY = (GetSystemMetrics(SM_CYSCREEN) - m_windowHeight) / 2;


	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowExW(0, wcex.lpszClassName, s2ws(m_name).c_str(),
		//WS_OVERLAPPEDWINDOW,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		//CW_USEDEFAULT, CW_USEDEFAULT,
		m_windowPosX, m_windowPosY,
		m_windowWidth, m_windowHeight, nullptr, nullptr, m_hInstance, nullptr);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

//-----------------------------------------------------------------------------

void App::shutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	//*
	// Fix the display settings if leaving full screen mode.
	if (m_fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	//*/

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(g_windowClassName, m_hInstance);
	m_hInstance = NULL;

	return;
}

//-----------------------------------------------------------------------------
/**
 * Sets the current app state
 *
 * @param {AppState} p_state The new state for the application
 */
void App::setCurrentState(AppState* p_state)
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

//-----------------------------------------------------------------------------
/**
 * Advances the application one tick forward,
 * which involves updating its components, and rendering a frame
 */
void App::tick()
{
	/*
	GetLogStream(SeverityInfo)
		<< "App::tick()" << std::endl;
	//*/

	m_stepTimer.Tick([&]()
	{
		AppState* nextState = m_currentState->update(m_stepTimer);
		if (nullptr != nextState) {
			setCurrentState(nextState);
		}

		/*
		GetLogStream(SeverityInfo)
			<< "App::tick() : elapsedSeconds = " << elapsedSeconds
			<< ", m_updateInterval = " << m_updateInterval << std::endl;
		//*/
	});

	m_currentState->draw();
}

//-----------------------------------------------------------------------------
/**
 *
 */
void App::shutdown()
{
	GetLogStream(SeverityInfo) << "AppBase::shutdown()" << std::endl;

	handleCleanUp();

	// TODO ? Will probably need code to shutdown things here

	// Do de-initializations here for the managers ans helper classes
}

//-----------------------------------------------------------------------------

void App::onActivated()
{
	GetLogStream(SeverityDebug) << "AppBase::onActivated()" << std::endl;
}

//-----------------------------------------------------------------------------

void App::onDeactivated()
{
	GetLogStream(SeverityDebug) << "AppBase::onDeactivated()" << std::endl;
}

//-----------------------------------------------------------------------------

void App::onSuspending()
{
	GetLogStream(SeverityDebug) << "AppBase::onSuspending()" << std::endl;
}

//-----------------------------------------------------------------------------

void App::onResuming()
{
	GetLogStream(SeverityDebug) << "AppBase::onResuming()" << std::endl;
	m_stepTimer.ResetElapsedTime();
}

//-----------------------------------------------------------------------------

void App::onWindowSizeChanged()
{
	/*
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
	//*/
}

} // namespace nxn
