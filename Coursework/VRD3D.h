#pragma once
#include "E:\Source\Repos\Abertay\persodral\DXFramework\D3D.h"
#include "../OpenVR/openvr.h"
#include <string>
#include "../DXFramework/RenderTexture.h"
#include "../ApplicationSettings.h"
class VRD3D :
	public D3D
{
public:
	VRD3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear, bool isVR = true);
	~VRD3D();

	void UpdateHMDMatrixPose();

	XMMATRIX ConvertSteamVRMatrixToXMMartix(const vr::HmdMatrix34_t & matPose);

	XMMATRIX GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);


	void RenderToVR();
	void RenderToVR(RenderTexture* ret);


	RenderTexture* leftEye;
	RenderTexture* rightEye;
protected:	
	
	XMMATRIX GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);

	XMMATRIX GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);


	XMMATRIX mat4HMDPose;
	XMMATRIX mat4eyePosLeft;
	XMMATRIX mat4eyePosRight;

	XMMATRIX mat4ProjectionCenter;
	XMMATRIX mat4ProjectionLeft;
	XMMATRIX mat4ProjectionRight;


	vr::IVRSystem *pHMD;
	vr::IVRRenderModels * pRenderModels;

	vr::TrackedDevicePose_t rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	char rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class
	XMMATRIX rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	int iValidPoseCount;
	std::string strPoseClasses;                            // what classes we saw poses for this frame


 


};

