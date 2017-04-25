#include "LSystem.h"

#include "../imgui/imgui.h"

LSystem::LSystem(ID3D11Device *myDevice, XMINT2 startPoint)
{
	iterations = 0;


	startPos = startPoint;
	lenghtOfLine = 4;
	angle = 90;
	textureNeedingUpdated = false;
	nextBuf = new float[SIZE_OF_NETWORK * SIZE_OF_NETWORK * COLOUR_ON_TEXTURE];

	for (int i = 0; i < SIZE_OF_NETWORK; i++)
	{
		for (int j = 0; j < SIZE_OF_NETWORK; j++)
		{
			for (int k = 0; k < COLOUR_ON_TEXTURE; k++)
			{

				nextBuf[i + SIZE_OF_NETWORK* (j + SIZE_OF_NETWORK* k)] = 1.0f;
			}
		}
	}


	GenterateUpdateTexture(myDevice);

 }

LSystem::LSystem(ID3D11Device * device, int itertaions, int lineLength, std::unordered_map<char, std::string> rules, const char * start, XMINT2 startPoint) : LSystem(device, startPoint)
 {
	this->start = start;
	iterations = itertaions;
	lenghtOfLine = lineLength;
	textureNeedingUpdated = true;
	this->rules = rules;
	Update();






 }

 

LSystem::~LSystem()
{
	if (nextBuf)
	{
		delete[] nextBuf;
		nextBuf = nullptr;
	}
}

 
void LSystem::loadSystem(std::unordered_map<char, std::string> rules)
{
 	this->rules = rules;

}
 
 

std::string LSystem::RecursiveCreateProduct(std::string values, int interation)
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

	return RecursiveCreateProduct(newValue, interation);
}

void LSystem::updateNetwork(std::string product )
{

	delete[] nextBuf;
	nextBuf = nullptr;

	nextBuf = new float[SIZE_OF_NETWORK * SIZE_OF_NETWORK * COLOUR_ON_TEXTURE];

	for (int i = 0; i < SIZE_OF_NETWORK; i++)
	{
		for (int j = 0; j < SIZE_OF_NETWORK; j++)
		{
			for (int k = 0; k < COLOUR_ON_TEXTURE; k++)
			{

				nextBuf[i + SIZE_OF_NETWORK* (j + SIZE_OF_NETWORK* k)] = 1.0f;
			}
		}
	}

	//start point in middle of map
	int startPointX = startPos.x;
	int startPointY = startPos.y;

	// Used for position or angles
 
	points.clear();
	XMINT2 currentPos = XMINT2(startPointX, startPointY);
	float currentAngle = 0;
	points.push_back(currentPos);
	for (auto str : product)
	{


		if (str == '+')
		{
			currentAngle += angle;
			currentDirection++;
		}
		else if (str == '-')
		{
			currentAngle -= angle;

			currentDirection--;
		}
		else if (str == 'F' || str == 'A' || str == 'B')
		{

			XMINT2 newPoint = CaculateNewPoint(currentPos, currentDirection*angle, lenghtOfLine);
			std::vector<XMINT2> newpoints = BresenhamAlgorithm(currentPos, newPoint);

 
			if (newpoints.size() > 0)
			{
				for (int i = 0; i < newpoints.size() - 1; i++)
				{
					int x = SIZE_OF_NETWORK * COLOUR_ON_TEXTURE;
					int y = COLOUR_ON_TEXTURE;
					int index = x * newpoints[i].x + newpoints[i].y * y;

					if (newpoints[i].x > 0 && newpoints[i].y > 0)
					{
						if (index < SIZE_OF_NETWORK * SIZE_OF_NETWORK * COLOUR_ON_TEXTURE + 2)
						{
							nextBuf[index] = 0.0f;
							nextBuf[index + 1] = 0.0f;
							nextBuf[index + 2] = 0.0f;

						}
					}
 


				}
				points.insert(points.end(), newpoints.begin(), newpoints.end());
 				currentPos = newPoint;
			}

		}
		if (currentDirection == 4)
		{
			currentDirection = 0;
		}
		if (currentDirection == -1)
		{
			currentDirection = 3;
		}

		if (currentAngle < 0)
		{
			currentAngle = 360 - currentAngle;
		}
		else if (currentAngle > 360)
		{
			currentAngle = currentAngle - 360;
		}

		 


	}


 }

void LSystem::Gui_menu(bool * is_open)
{
	if (*is_open)
	{

		if (ImGui::InputFloat("Angle", &angle))
		{
			textureNeedingUpdated = true;

			Update();
		}
		if (ImGui::InputInt("Lenght Of Line", &lenghtOfLine))
		{
			textureNeedingUpdated = true;

			Update();
		}
		if (ImGui::InputInt2("Start Position", &startPos.x))
		{
			textureNeedingUpdated = true;

			Update();
		}
 
		if (ImGui::InputInt("Number Of Iterations", &iterations))
		{
			textureNeedingUpdated = true;
			Update();
		}
 
	}
}

void LSystem::GenterateUpdateTexture(ID3D11Device *myDevice)
{
	if (texture)
	{	
		texture->Release();	
		texture = 0;

	}
	D3D11_TEXTURE2D_DESC   tdesc;


	int width = SIZE_OF_NETWORK;
	int height = SIZE_OF_NETWORK;
	int colours = COLOUR_ON_TEXTURE;
	int index = 0;
	int f = 0;
	float* buf = new float[width * height * colours];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{

 
		   float colour = 1.0f;

 
			buf[index] = nextBuf[index];;
			index++;
			buf[index] = nextBuf[index];;;
			index++;

			buf[index] = nextBuf[index];;;
			index++;

 
		}
 	}
	texture = 	displayTexture->SetTexture( displayTexture->CreateDynamicTexture(myDevice,width,height,buf, tdesc),myDevice, &tdesc);

//	delete[] buf;
}

Texture * LSystem::getTexture()
{

	return displayTexture;
}

 
void LSystem::Update()
{
	if (textureNeedingUpdated)
	{
		textureNeedingUpdated = false;
		std::string product = RecursiveCreateProduct(start, iterations);
		updateNetwork(product);
	}
}

XMINT2 LSystem::CaculateNewPoint(XMINT2 startPoint, float angle, int lengthOfLine)
{

	// http://mathforum.org/library/drmath/view/61081.html
	XMINT2 endpos;


	angle = angle * (XM_PI / 180);
	endpos.x = startPoint.x + (lengthOfLine * cos( angle));
	endpos.y = startPoint.y + (lengthOfLine * sin(angle));

	return endpos;
}

std::vector<XMINT2> LSystem::BresenhamAlgorithm(XMINT2 startPos, XMINT2 endPos)
{

	int x1 = startPos.x;
	int y1 = startPos.y;
	int x2 = endPos.x;
	int y2 = endPos.y;

	std::vector<XMINT2> newPoints;

	int deltaX = endPos.x - startPos.x;
	int deltaY = endPos.y - startPos.y;
	float	error = 0;

	if (deltaY == 0 )
	{
		if (endPos.x > startPos.x)
		{
			for (int i = 0; i < deltaX; i++)
			{
				newPoints.push_back(XMINT2(startPos.x + i, startPos.y));
			}
		}
		else
		{
			for (int i = 0; i < abs(deltaX); i++)
			{
				newPoints.push_back(XMINT2(startPos.x - i, startPos.y));
			}
		}
	}else if (deltaX == 0)
	{
		if (endPos.y > startPos.y)
		{
			for (int i = 0; i < deltaY; i++)
			{
				newPoints.push_back(XMINT2(startPos.x, startPos.y+i));
			}
		}
		else
		{
			for (int i = 0; i < abs(deltaY); i++)
			{
				newPoints.push_back(XMINT2(startPos.x, startPos.y -i));
			}
		}
	 

	}
	else
	{// Note the below fails for completely vertical lines.
		float deltaError = abs(deltaY / deltaX);

		float y = startPos.y;
		float x;
		int direction = 0;
		if (startPos.x > endPos.x)
		{

			for (int x = startPos.x; x < endPos.x; x++)
			{
				newPoints.push_back(XMINT2(x, y));
				error = error + deltaError;
				if (error >= 0.5)
				{
					y++;

					error -= 1.0;
				}
			}
		}
		else
		{
			for (int x = startPos.x; x > endPos.x; x--)
			{
				error = error + deltaError;
				if (error >= 0.5)
				{
					y++;

					error -= 1.0;
				}
			}
		}

	}
	return newPoints;
}
