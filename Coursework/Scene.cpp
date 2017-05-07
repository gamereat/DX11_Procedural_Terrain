#include "Scene.h"

#include "../DXFramework/BaseApplication.h"

#include "../ApplicationSettings.h"
/* See the file "LICENSE" for the full license governing this code. */

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
}

Scene::~Scene()
{
	if (depthTexture)
	{
		delete depthTexture;
		depthTexture = nullptr;
	}
}

void Scene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext, Sound * sound)
{	depthTexture = new RenderTexture(device, ApplicationSettings::screenWidth, ApplicationSettings::screenHeight, ApplicationSettings::sceenNear, ApplicationSettings::screenDepth);
	this-> sound = sound;
}

 

void Scene::ResetLights(Light* lights[])
{
}

std::string Scene::getSceneName()
{
	return sceneName;
}

