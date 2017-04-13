#include "VRD3D.h"

VRD3D::VRD3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear, bool isVR)
: D3D(screenWidth, screenHeight,vsync,hwnd,fullscreen,screenDepth,screenNear)
{
	if (isVR)
	{
 

		//// Loading the SteamVR Runtime
		vr::EVRInitError eError = vr::VRInitError_None;

		pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

		if (eError != vr::VRInitError_None)
		{
			pHMD = NULL;
		}
		uint32_t width = ApplicationSettings::screenWidth;
		uint32_t hieght = ApplicationSettings::screenHeight;

		pHMD->GetRecommendedRenderTargetSize(&width, &hieght);

		//ApplicationSettings::screenWidth = width;
		//ApplicationSettings::screenHeight = hieght;


		leftEye = new RenderTexture(device, screenWidth, screenHeight, ApplicationSettings::sceenNear, screenDepth);
		rightEye = new RenderTexture(device, screenWidth, screenHeight, ApplicationSettings::sceenNear, screenDepth);


		pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
		if (!pRenderModels)
		{
			pHMD = NULL;
			vr::VR_Shutdown();

		}
		mat4ProjectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
		mat4ProjectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
		mat4eyePosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
		mat4eyePosRight = GetHMDMatrixPoseEye(vr::Eye_Right);

		if (!vr::VRCompositor())
		{

		}
	}
}
 

VRD3D::~VRD3D()
{


	
}


void VRD3D::UpdateHMDMatrixPose()
{
	if (!pHMD)
		return;

	vr::VRCompositor()->WaitGetPoses(rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	iValidPoseCount = 0;
	strPoseClasses = "";
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			iValidPoseCount++;
			rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToXMMartix(rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (rDevClassChar[nDevice] == 0)
			{
				switch (pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_Other:             rDevClassChar[nDevice] = 'O'; break;
				case vr::TrackedDeviceClass_TrackingReference: rDevClassChar[nDevice] = 'T'; break;
				default:                                       rDevClassChar[nDevice] = '?'; break;
				}
			}
			strPoseClasses += rDevClassChar[nDevice];
		}
	}

	if (rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		mat4HMDPose = XMMatrixInverse(nullptr, rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
	}
	else
	{
	}
}

XMMATRIX VRD3D::ConvertSteamVRMatrixToXMMartix(const vr::HmdMatrix34_t &matPose)
{
	XMMATRIX matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}

XMMATRIX VRD3D::GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	XMMATRIX matMVP;
	if (nEye == vr::Eye_Left)
	{
		matMVP = mat4ProjectionLeft * mat4eyePosLeft * mat4HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = mat4ProjectionRight * mat4eyePosRight * mat4HMDPose;
	}

	//dprintf("pleft = %s\n, eyeLeft = %s\n, hmdPose = %s\n",
	//	MatrixToString(m_mat4ProjectionLeft).c_str(),
	//	MatrixToString(m_mat4eyePosLeft).c_str(),
	//	MatrixToString(m_mat4HMDPose).c_str());

	return matMVP;
}

void VRD3D::RenderToVR()
{
	m_swapChain->Present(0, 0);


	vr::Texture_t leftEyeTexture = { leftEye->GetTexture(), vr::API_DirectX, vr::ColorSpace_Auto };
	vr::EVRCompositorError error1 = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { rightEye->GetTexture(), vr::API_DirectX, vr::ColorSpace_Auto };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);


	UpdateHMDMatrixPose();

}

 

void VRD3D::RenderToVR(RenderTexture* ret)
{


	//vr::Texture_t leftEyeTexture = { ret->GetTexture(), vr::API_DirectX, vr::ColorSpace_Auto };
	//vr::EVRCompositorError error1 = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
//	vr::Texture_t rightEyeTexture = { ret->GetTexture(), vr::API_DirectX, vr::ColorSpace_Auto };
	//vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);



	UpdateHMDMatrixPose();

}

XMMATRIX  VRD3D::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!pHMD)
		return XMMATRIX();

	vr::HmdMatrix34_t matEyeRight = pHMD->GetEyeToHeadTransform(nEye);
	XMMATRIX matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return XMMatrixInverse(nullptr,matrixObj);
}

XMMATRIX  VRD3D::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!pHMD)
		return XMMATRIX();

	vr::HmdMatrix44_t mat = pHMD->GetProjectionMatrix(nEye, ApplicationSettings::sceenNear,30.0f, vr::API_DirectX);

	return XMMATRIX(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}