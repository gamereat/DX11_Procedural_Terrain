#include "ApplicationSettings.h"
#include "../MiniIni/minIni.hpp"


bool ApplicationSettings::isVSync = false;
bool ApplicationSettings::isFullScreen = false;
int ApplicationSettings::screenHeight = 800;
int ApplicationSettings::screenWidth = 600;
float ApplicationSettings::sceenNear = 0.1;
int ApplicationSettings::screenDepth = 200;
ApplicationSettings::ApplicationSettings()
{
}


ApplicationSettings::~ApplicationSettings()
{
}

void ApplicationSettings::Init(BaseApplication* baseApp)
{
	InitGraphicSetting(baseApp);
}

void ApplicationSettings::InitGraphicSetting(BaseApplication* baseApp)
{

	minIni graphics("Settings-Files/Graphics.ini");
	baseApp->screenWidth = screenWidth  = graphics.geti("Window", "width");
	baseApp->screenHeight = screenHeight = graphics.geti("Window", "height");
	baseApp->sceenNear = sceenNear = graphics.getf("Window", "near");
	baseApp->screenDepth = screenDepth = graphics.geti("Window", "depth");
	baseApp->isFullScreen = isFullScreen = graphics.getbool("Window", "fullScreen");
	baseApp->isVsyncEnabled = isVSync = graphics.getbool("Window", "vsync");
}
