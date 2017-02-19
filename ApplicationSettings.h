#pragma once
#include "../DXFramework/BaseApplication.h"

/*
Application Settings 
@Author Alan Yeats

The application settings for the given gam
*/
class ApplicationSettings
{
public:
	ApplicationSettings();
	~ApplicationSettings();

	/*
	Init the given applicaiton settings
	*/
	static void Init(BaseApplication* baseApp);

	/*
	Screen width for the application
	*/
	static int screenWidth;

	/*
	Screen height for the application
	*/
	static int screenHeight;

	/*
	Screen near disatance for the application
	*/
	static float sceenNear;

	/*
	Screen depth for the application
	*/
	static int screenDepth;

	/*
	if the applicaiton is full screen
	*/
	static bool isFullScreen;


	/*
	if application using v-sysnc
	*/
	static bool isVSync;

private:
	/*
	Init graphics settings

	@param baseApp		The base application being used
	*/
	static void InitGraphicSetting(BaseApplication* baseApp);
};

