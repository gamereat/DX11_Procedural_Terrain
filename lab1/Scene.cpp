#include "Scene.h"

#include "../DXFramework/BaseApplication.h"

#include "../ApplicationSettings.h"

Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
}

Scene::~Scene()
{
}

void Scene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{


	depthTexture = new RenderTexture(device, ApplicationSettings::screenWidth, ApplicationSettings::screenHeight, ApplicationSettings::sceenNear, ApplicationSettings::screenDepth);

}

void Scene::ResetLights(Light* lights[])
{
}

std::string Scene::getSceneName()
{
	return sceneName;
}

