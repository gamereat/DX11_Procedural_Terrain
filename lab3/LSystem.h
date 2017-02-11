#pragma once
#include <unordered_map>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
using namespace DirectX;

typedef void(*LShapedConstant)(int iteration);
class LSystem
{
public:
	LSystem();
	~LSystem();

	void loadSystem( std::unordered_map<char, LShapedConstant> constant, std::unordered_map<char, std::string> rules  );

	std::string recustionValues(std::string values, int interation);
	void Gui_menu(bool *is_open);
private:
	std::vector<XMFLOAT2> points;
	int iterations;
	char start;

	std::unordered_map<char, LShapedConstant> constants;
	std::unordered_map<char , std::string> rules;

};

