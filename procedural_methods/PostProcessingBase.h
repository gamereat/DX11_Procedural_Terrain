#pragma once
#include "../DXFramework/RenderTexture.h"
#include "../DXFramework/baseapplication.h"
#include "../DXFramework/OrthoMesh.h"

#include "../DXFramework/D3D.h"
class PostProcessingBase
{
public:
	PostProcessingBase(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProcessingBase();

	//virtual void Init(D3D* directX3D, HWND hwnd, float downscaleValue) =0;

	virtual RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture) = 0;

	virtual void MenuOptions(bool* isOpen) =0 ;

	void ToggleMenu();
protected:

	/*
	Called before the at the begining of the render function creating all the martix informaiton and setting up for post processing
	*/
	void PreRender(D3D* directX3D, Camera* camera, XMMATRIX& worldMatrix, XMMATRIX& baseViewMatrix, XMMATRIX& orthoMartix, OrthoMesh* orthoMesh);


	/*
	Called at the end of Render function does tidying up of the rendering
	*/
	void PostRender(D3D* directX3D);

	/*
	Rendering Texture used for the post Proccessing effect
	*/
	RenderTexture* renderTexture;


	bool isEnabled;	/*

	*/
	bool isMenuOpen;
private:


};

