#pragma once
#include <unordered_map>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "../DXFramework/Texture.h"
#include <dxgi.h>
#include <DirectXMath.h>
using namespace DirectX;

 
const int SIZE_OF_NETWORK = 1000;
const int COLOUR_ON_TEXTURE = 3;
typedef void(*LShapedConstant)(int iteration);


/*
L-System
L-system object creates a texture based off a given l-system rule set
Resourced : https://en.wikipedia.org/wiki/L-system
*/
class LSystem
{
public:
	/*
	Constructor creates l-system texture 
	@param device	The device for the given d3d window
	*/
	LSystem(ID3D11Device *device, XMINT2 startPoint = XMINT2(SIZE_OF_NETWORK / 2, SIZE_OF_NETWORK / 2));


	/*
	Constructor creates l-system texture
	@param device		The device for the given d3d window
	@param iterations	The number of iterations applicaion should start with
	@param lineLength	How long each line should be used when using turle graphics
	@param rules		The rules for the given l=system
	*/
	LSystem(ID3D11Device * device, int itertaions, int lineLength, std::unordered_map<char, std::string> rules, const char * start, XMINT2 startPoint = XMINT2(SIZE_OF_NETWORK/2, SIZE_OF_NETWORK/2));
	~LSystem();

	/*
	Load System 
	Loads in the settings required to create l-system
	*/
	void loadSystem(  std::unordered_map<char, std::string> rules  );

	/*
	Base GUI settings created using UMGUI
	Setting that can effect the way the l-system looks
	*/
	void Gui_menu(bool *is_open);

	/*
	Regenerate the latest texture from the l-System

	@param device		The current D3D device for the window
	*/
	void GenterateUpdateTexture(ID3D11Device * device);
	 
 	Texture* getTexture();

	/*
	The texture the l-sytem is being displayed on
	*/
	ID3D11ShaderResourceView* texture;


	/*
	Will update the given l-system with the latest settings
 	*/
	void Update();
	/*
	If the texture needs to be updated. ie if any settings have changd on it 
	*/
	bool textureNeedingUpdated;
private:


	float* nextBuf ;


	const char* start;






	/*
	Updates the current network based of the product creating the points that the l-system covers
	@param product		The product being created 
	*/
	void updateNetwork(std::string product);
	/*
	recursivivly create product based on number of interation completed
	@param	values		The current product 
	@param interation	The number of iteration current at 
	*/
	std::string RecursiveCreateProduct(std::string values, int interation);

	/*
	Caculates the next point in a line. Based off the angle required and its start point 
	*/
	XMINT2 CaculateNewPoint(XMINT2 startPoint, float angle, int lengthOfLine);
	/*
	Will add pixels better two given points on the texture.
	Refrenced from Bresenham is Fast and Good chapter 35
	http://downloads.gamedev.net/pdf/gpbb/gpbb35.pdf
	*/
	std::vector<XMINT2> BresenhamAlgorithm(XMINT2 startPos,XMINT2 endPos);



	/*
	The current product that has been creatd using the l-system
	*/
	std::string product;

	/*
	
	*/
	int currentDirection = 0;

	/*
	The angle that is being used for the l-system
	*/
	float angle;

	/*
	Texture being used to create texture from
	*/
	Texture* displayTexture;

	/*
	The points on the texture that will be coloured for the l-system
	*/
	std::vector<XMINT2> points;
	/*
	The number of iteration to be created
	*/
	int iterations;
	
	/*
	The set of rules the l-system is made using
	*/
 	std::unordered_map<char , std::string> rules;

	/*
	Starting position on the texture the l-system will be drawn from
	*/
	XMINT2 startPos;

	/*
	How far forward the turtle will draw for a given constant 
	*/
	int lenghtOfLine;
};

