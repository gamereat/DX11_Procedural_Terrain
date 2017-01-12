//System.cpp
#include "system.h"


System::System(BaseApplication* application)
{
	int screenWidth, screenHeight;
	
	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the application wrapper object.
	//m_Application = new Lab4s(m_hinstance, m_hwnd, screenWidth, screenHeight, &m_Input);
	m_Application = application;
	m_Application->init(m_hinstance, m_hwnd, screenWidth, screenHeight, &m_Input);


}


System::~System()
{
	// Release the application wrapper object.
	if (m_Application)
	{
		//m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
}


void System::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool System::Frame()
{
	bool result;


	// Do the frame processing for the application object.
	result = m_Application->Frame();

	if (!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Alan Yeats DirectX";

	//// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
 
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
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
		screenWidth = SCREEN_WIDTH;
		screenHeight = SCREEN_HEIGHT;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
 
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void System::ShutdownWindows()
{

	ImGui_ImplDX11_Shutdown();

	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	switch (umessage)
	{
		// Handle keyboard and mouse input.
		case WM_KEYDOWN:
		{
			ApplicationHandle->m_Input.SetKeyDown(wparam);
			break;
		}
		case WM_KEYUP:
		{
			ApplicationHandle->m_Input.SetKeyUp(wparam);
			break;
		}
		case WM_MOUSEMOVE:
		{
			ApplicationHandle->m_Input.setMouseX(LOWORD(lparam));
			ApplicationHandle->m_Input.setMouseY(HIWORD(lparam));
			break;
		}
		case WM_LBUTTONDOWN:
		{
			ApplicationHandle->m_Input.setLeftMouse(true);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			ApplicationHandle->m_Input.setRightMouse(true);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			ApplicationHandle->m_Input.setMiddleMouse(true);
			break;
		}
		case WM_LBUTTONUP:
		{
			ApplicationHandle->m_Input.setLeftMouse(false);
			break;
		}
		case WM_RBUTTONUP:
		{
			 ApplicationHandle->m_Input.setRightMouse(false);
			 break;
		}

		case WM_MBUTTONUP:
		{
			ApplicationHandle->m_Input.setMiddleMouse(false);
			break;
		}
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

		
	}
	if (ImGui_ImplDX11_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}