#pragma once
#include "Scene.h"
#include "../DXFramework/SphereMesh.h"
#include "../DXFramework/CubeMesh.h"
#include "../DXFramework/QuadMesh.h"
#include "../DXFramework/DoubleTriangleMesh.h"
#include "LSystemShader.h"
#include "ColourShader.h"
#include "Terrain.h"
#include "../DXFramework/CubeMesh.h"
#include "TextureShader.h"
#include "../DXFramework/Model.h"
#include "Sound.h"
#include "TerrainShader.h"
 #include "LSystem.h"

const int NUMBER_LSYSTEMS =11;


//http://algorithmicbotany.org/papers/abop/abop-ch1.pdf
enum lSystemTypes
{
	DragonCurve,
	KochCurve,
	SierpinskisCarpet,
	HeighwayDragon,
	JoinedCrossCurves,
	modifiedKoch1,
	modifiedKoch2,
	modifiedKoch3,
	modifiedKoch4,
	modifiedKoch5,
	modifiedKoch6,
};
/*
L-System Scene
@author Alan Yeats
L-System scene used to display a L-system Represented on a dynamic texture on a plane
*/
 class LSystemScene :	public Scene
{const char* lSystemTypes_str[NUMBER_LSYSTEMS]
{
	"Dragon curve",
	"Koch curve",
	"Sierpinski's Carpet",
	"Heighway Dragon",
	"Joined Cross Curves",
	"Modified Koch Curve 1",
	"Modified Koch Curve 2",
	"Modified Koch Curve 3",
	"Modified Koch Curve 4",
	"Modified Koch Curve 5",
	"Modified Koch Curve 6",
 };
public:
	LSystemScene(string sceneName);
	~LSystemScene();
  	/*
	Initializes the given Scene

	@param hwnd		A handle to the window
	@param device	The directx11 device
	@param deviceContext	The directx11 device Context

	*/
	virtual void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Sound* sound);

	/*
	Updates the current scene

	@param timer	 application timer that can be used to get total time and delta time for application
	*/
	void Update(Timer* timer);
	/*
	Renders the given scene

	@param renderTexture	The given render texture that should be used for this scene
	@param device			The directx11 device
	@paran camera			the camera for the scene
	@param depthmap			The Render textures to be used for depths maps from the given lights
	@param lights			The lights within the scene
	*/
	void Render(RenderTexture* renderTexture, D3D* device, Camera* camera, RenderTexture *depthMap[], Light* light[]);

	/*
	The given menu options for the scene
	*/
	void MenuOptions();


	/*
	Will set lights back to correct position in the world
	Used when changing scene so lights so the scene in the best light :D and are light correctly

	@param lights The lights from within the world
	*/
	virtual void ResetLights(Light* lights[]);

	/*
	Does a depth pass on the given scene

	@param device	The directx11 device
	@param camera	The camera within the scene
	@param DepthMap	The depth maps to be created on to
	@param lights   The lights within the scene
	*/
	void GenerateDepthPass(D3D* device, Camera* camera, RenderTexture*depthMap[], Light* light[]);

	/*
	Scene information pop up will tell the user about the current scene
  	*/
	virtual void SceneInformationPopUp(bool* is_open);
	 

private:


	void LSystemMenu(bool* is_open);

	lSystemTypes currentLSystem;
	
	LSystemShader* lSystemShader; 

	CubeMesh* cube;
	/*
	If the l-system needs to be updated or not 
	*/
	bool updateLSysteam;
	/*
	The number of iterations in the l-System
	*/
	int numberOfIterations;
	/*
	Plane meshed used to display the l-sytem on 
	*/
	PlaneMesh* planeMesh;


 	/*
	L-system object that creates texture based on the l system rules
	*/
	LSystem* lSystem[NUMBER_LSYSTEMS];
	/*
	Texture shader 
	*/
 	TextureShader* textureShader;
	/*
	Colour shader
	*/
	ColourShader* colourShader;
 
	/*
	Scaling used by the plane to display on screen
	*/
	XMFLOAT3 planeScale;
	/*
	Rotation used by the plane to display on screen
	*/
	XMFLOAT3 planeRotate;
	/*
	Translation used by the plane to display on screen
	*/
	XMFLOAT3 planeTranslation;
 };

