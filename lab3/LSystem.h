#pragma once
#include <unordered_map>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "../DXFramework/Texture.h"
#include <dxgi.h>
#include <DirectXMath.h>
using namespace DirectX;

struct positionInformation
{
	float rotation;
	XMFLOAT2 postion;

};
const int SIZE_OF_NETWORK = 1000;
typedef void(*LShapedConstant)(int iteration);



class LSystem
{
public:
	LSystem(ID3D11Device *myDevice);
	~LSystem();

	void loadSystem( std::unordered_map<char, LShapedConstant> constant, std::unordered_map<char, std::string> rules  );

	std::string recustionValues(std::string values, int interation);
	void updateNetwork(std::string);
	void Gui_menu(bool *is_open);

	void GenterateUpdateTexture(ID3D11Device * myDevice, int f);

	bool network[SIZE_OF_NETWORK][SIZE_OF_NETWORK];

	//void GenterateUpdateTexture(ID3D11Device *myDevice);
	Texture* getTexture();
	ID3D11ShaderResourceView* tes;


	void GUI_Settings(bool *is_open);
private:

	float angle;
	Texture* displayTexture;
	std::vector<XMFLOAT2> points;
	int iterations;
	char start;

	std::unordered_map<char, LShapedConstant> constants;
	std::unordered_map<char , std::string> rules;


};

