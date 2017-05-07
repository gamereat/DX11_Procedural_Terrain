// Main.cpp
#include "../DXFramework/System.h"
#include "BaseApp.h"
#include "../ApplicationSettings.h"
/* See the file "LICENSE" for the full license governing this code. */


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
 


	BaseApp* app = new BaseApp();
	System* m_System;
	ApplicationSettings::Init(app);

 	// Create the system object.
	m_System = new System(app);

	// Initialize and run the system object.
	m_System->Run();

	// Shutdown and release the system object.
	delete m_System;
	m_System = 0;

	return 0;
}