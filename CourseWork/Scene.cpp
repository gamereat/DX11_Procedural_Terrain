#include "Scene.h"

#include "../DXFramework/BaseApplication.h"
 


Scene::Scene(std::string sceneName)
{
	this->sceneName = sceneName;
}

Scene::~Scene()
{
}

void Scene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{

	depthShader = new DepthShader(device, hwnd);
	depthTexture = new RenderTexture(device, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

}

void Scene::ResetLights(Light* lights[])
{
}

std::string Scene::getSceneName()
{
	return sceneName;
}

