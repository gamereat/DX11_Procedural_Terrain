// System.h
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

// INCLUDES 
#include <windows.h>
#include "baseapplication.h"

//#include "Lab4s.h"
#include "Input.h"

class System
{
public:
	System(BaseApplication* application);
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	//Application* m_Application;
	BaseApplication* m_Application;
	Input m_Input;

	//Prototypes
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

//Globals
static System* ApplicationHandle = 0;

#endif