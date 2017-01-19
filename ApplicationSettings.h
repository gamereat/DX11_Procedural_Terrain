#pragma once
#include "../DXFramework/BaseApplication.h"
class ApplicationSettings
{
public:
	ApplicationSettings();
	~ApplicationSettings();

	static void Init(BaseApplication* baseApp);

	static int screenWidth;
	static int screenHeight;
	static float sceenNear;
	static int screenDepth;
	static bool isFullScreen;
	static bool isVSync;

private:
	static void InitGraphicSetting(BaseApplication* baseApp);
};

