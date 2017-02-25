#include "LSystem.h"

#include "../imgui/imgui.h"

LSystem::LSystem(ID3D11Device *myDevice)
{
	iterations = 0;

	GenterateUpdateTexture(myDevice);

	startPos.x = SIZE_OF_NETWORK / 4;
	startPos.y = SIZE_OF_NETWORK / 2;
	lenghtOfLine = 4;
	angle = 90;
	textureNeedingUpdated = false;
 }


LSystem::LSystem(ID3D11Device * device, int itertaions, int lineLength, std::unordered_map<char, std::string> rules) : LSystem(device)
{
	iterations = itertaions;
	lenghtOfLine = lineLength;
	textureNeedingUpdated = true;
	this->rules = rules;
	Update();
}

LSystem::~LSystem()
{
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

 
			points.insert(points.end(), newpoints.begin(), newpoints.end());
			//points.push_back(newPoint);
			currentPos = newPoint;

//
//			if (currentDirection == 0)
//			{
//				std::vector<XMINT2> newpoints = BresenhamAlgorithm(currentPos, XMINT2(currentPos.x,currentPos.y +4));
//
//
//				points.insert(points.end(), newpoints.begin(), newpoints.end());
//				currentPos.y += 4;
//
//			//	points.push_back(XMINT2(startPointX, startPointY++));
//			//	points.push_back(XMINT2(startPointX, startPointY++));
//
//			//	points.push_back(XMINT2(startPointX, startPointY++));
//			//	points.push_back(XMINT2(startPointX, startPointY++));
//			}
//			else if (currentDirection == 1)
//			{
//				std::vector<XMINT2> newpoints = BresenhamAlgorithm(currentPos, XMINT2(currentPos.x -4, currentPos.y ));
//
//
//				points.insert(points.end(), newpoints.begin(), newpoints.end());
//				currentPos.x -= 4;
//
//				//points.push_back(XMINT2(startPointX--, startPointY));
//				//points.push_back(XMINT2(startPointX--, startPointY));
//				//points.push_back(XMINT2(startPointX--, startPointY));
//				//points.push_back(XMINT2(startPointX--, startPointY));
//			}
//			else if (currentDirection == 2)
//			{
//
//				std::vector<XMINT2> newpoints = BresenhamAlgorithm(currentPos, XMINT2(currentPos.x, currentPos.y - 4));
//
//
//				points.insert(points.end(), newpoints.begin(), newpoints.end());
//				currentPos.y -= 4;
//
//				//points.push_back(XMINT2(startPointX, startPointY--));
//				//points.push_back(XMINT2(startPointX, startPointY--));
//				//points.push_back(XMINT2(startPointX, startPointY--));
//				//points.push_back(XMINT2(startPointX, startPointY--));
//			}
//			else if (currentDirection == 3)
//			{
//				std::vector<XMINT2> newpoints = BresenhamAlgorithm(currentPos, XMINT2(currentPos.x +4, currentPos.y ));
//
//
//				points.insert(points.end(), newpoints.begin(), newpoints.end());
//				currentPos.x += 4;
///*
//				points.push_back(XMINT2(startPointX++, startPointY));
//				points.push_back(XMINT2(startPointX++, startPointY));
//				points.push_back(XMINT2(startPointX++, startPointY));
//				points.push_back(XMINT2(startPointX++, startPointY));*/
//			}
////
		 

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
		// Create the window
		if (!ImGui::Begin("L-System ", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
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
		ImGui::End();

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
	texture = 	displayTexture->SetTexture( displayTexture->CreateDynamicTexture(myDevice,width,height,buf, tdesc),myDevice, &tdesc);

	delete[] buf;
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
		std::string product = RecursiveCreateProduct("F", iterations);
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




	//----------------
	//const bool steep = (fabs(endPos.y - startPos.y) > fabs(endPos.x - startPos.x));
	//if (steep)
	//{
	//	std::swap(startPos.x, startPos.y);
	//	std::swap(endPos.x, endPos.y);
	//}

	//if (startPos.x > endPos.x)
	//{
	//	std::swap(startPos.x, endPos.x);
	//	std::swap(startPos.y, endPos.y);
	//}

	//const float dx = endPos.x - startPos.x;
	//const float dy = fabs(endPos.y - startPos.y);

	//float error = dx / 2.0f;
	//const int ystep = (startPos.y < endPos.y) ? 1 : -1;
	//int y = (int)startPos.y;

	//const int maxX = (int)endPos.x;

	//for (int x = (int)startPos.x; x<maxX; x++)
	//{
	//	if (steep)
	//	{
	//		newPoints.push_back(XMINT2(x, y));

 //		}
	//	else
	//	{
	//		newPoints.push_back(XMINT2(x, y));

 //		}

	//	error -= dy;
	//	if (error < 0)
	//	{
	//		y += ystep;
	//		error += dx;
	//	}
	//}
	//	------------------


 

//-----

	//int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	//dx = endPos.x - startPos.x;
	//dy = endPos.y - startPos.y;
	//dx1 = fabs(dx);
	//dy1 = fabs(dy);
	//px = 2 * dy1 - dx1;
	//py = 2 * dx1 - dy1;
	//if (dy1 <= dx1)
	//{
	//	if (dx >= 0)
	//	{
	//		x = startPos.x;
	//		y = startPos.y;
	//		xe = endPos.x;
	//	}
	//	else
	//	{
	//		x = endPos.x;
	//		y = endPos.y;
	//		xe = startPos.x;
	//	}
	//	newPoints.push_back(XMINT2(x, y));
	//	for (i = 0; x<xe; i++)
	//	{
	//		x = x + 1;
	//		if (px<0)
	//		{
	//			px = px + 2 * dy1;
	//		}
	//		else
	//		{
	//			if ((dx<0 && dy<0) || (dx>0 && dy>0))
	//			{
	//				y = y + 1;
	//			}
	//			else
	//			{
	//				y = y - 1;
	//			}
	//			px = px + 2 * (dy1 - dx1);
	//		}
	//		newPoints.push_back(XMINT2(x, y));
	//	}
	//}
	//else
	//{
	//	if (dy >= 0)
	//	{
	//		x = startPos.x;
	//		y = startPos.y;
	//		ye = endPos.y;
	//	}
	//	else
	//	{
	//		x = endPos.x;
	//		y = endPos.y;
	//		ye = startPos.y;
	//	}
	//	newPoints.push_back(XMINT2(x, y));
	//	for (i = 0; y < ye; i++)
	//	{
	//		y = y + 1;
	//		if (py <= 0)
	//		{
	//			py = py + 2 * dx1;
	//		}
	//		else
	//		{
	//			if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
	//			{
	//				x = x + 1;
	//			}
	//			else
	//			{
	//				x = x - 1;
	//			}
	//			py = py + 2 * (dx1 - dy1);
	//		}
	//		newPoints.push_back(XMINT2(x, y));
	//	}
	//}
	//
	return newPoints;
}
