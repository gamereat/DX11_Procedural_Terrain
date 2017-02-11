#include "LSystem.h"



LSystem::LSystem()
{
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

void LSystem::Gui_menu(bool * is_open)
{
}
