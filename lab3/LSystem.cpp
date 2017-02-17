#include "LSystem.h"

#include "../imgui/imgui.h"

LSystem::LSystem(ID3D11Device *myDevice)
{
	GenterateUpdateTexture(myDevice,0);
}


LSystem::~LSystem()
{
}

void LSystem::loadSystem(std::unordered_map<char, LShapedConstant> constant, std::unordered_map<char, std::string>rules)
{
	this->constants = constant;
	this->rules = rules;
	 


}
 
std::string LSystem::recustionValues(std::string values, int interation)
{

	if (interation <= 0)
	{
		return values;
	}

	std::string newValue;

	for (auto itr : values)
	{
 

		if (rules.find(itr) != rules.end())
		{
			newValue += rules[itr];
		}
		else
		{
			newValue += itr;
		}

	}
	interation -= 1;

	return recustionValues(newValue, interation);
}

void LSystem::updateNetwork(std::string product )
{
	//start point in middle of map
	int startPointX = SIZE_OF_NETWORK / 2;
	int startPointY = SIZE_OF_NETWORK / 2;

	// Used for position or angles
	positionInformation* currentStack;

	points.clear();

	int currentDirection = 0;
	for (auto str : product)
	{


		if (str == '+')
		{
			currentDirection++;
		}
		else if (str == '-')
		{
			currentDirection--;
		}
		else if (str == 'F')
		{
			if (currentDirection == 0)
			{
				startPointY++;

			}
			else if (currentDirection == 1)
			{
				startPointX--;
			}
			else if (currentDirection == 2)
			{
				startPointY--;

			}
			else if (currentDirection == 3)
			{
				startPointX++;
			}
			points.push_back(XMFLOAT2(startPointX, startPointY));

		}
		if (currentDirection == 4)
		{
			currentDirection = 0;
		}

		if (constants.find(str) != constants.end())
		{
 
		}


	}





}

void LSystem::Gui_menu(bool * is_open)
{
	if (*is_open)
	{
		// Create the window
		if (!ImGui::Begin("L-System ", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::InputFloat("Angle", &angle);

		ImGui::End();

	}
}

void LSystem::GenterateUpdateTexture(ID3D11Device *myDevice,int ggg)
{

	D3D11_TEXTURE2D_DESC   tdesc;


	int width = SIZE_OF_NETWORK;
	int height = SIZE_OF_NETWORK;
	int colours = 3;
	int index = 0;
	int f = 0;
	float* buf = new float[width * height * colours];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float colour = 1.0f;

			for (auto pt : points)
			{
				if (pt.x == i && pt.y == j)
				{
					colour = 0.0f;
				}
			}
		 
			buf[index] = colour;
			index++;
			buf[index] = colour;
			index++;
			buf[index] = colour;
			index++;

						
			
 
		}
 	}
	tes = 	displayTexture->SetTexture( displayTexture->CreateDynamicTexture(myDevice,width,height,buf, tdesc),myDevice, &tdesc);

	delete[] buf;
}

Texture * LSystem::getTexture()
{

	return displayTexture;
}

void LSystem::GUI_Settings(bool * is_open)
{
}
