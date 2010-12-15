#include "AntLarva.h"


namespace AntZerg
{
	AntLarva::AntLarva(const std::string& configFile, const std::string& actionScriptFile, const float x, const float y)
		: Ant(configFile, actionScriptFile, x, y), movementEnabled(true), food(0),
		numTimesFoodEaten(0), maxFoodBeforeMorph(1), morph(false)
	{
	}

	AntLarva::~AntLarva()
	{
	}

	void AntLarva::DecreaseFoodStock()
	{
		if(food > 0)
		{
			food--;
		}
	}

	void AntLarva::Eat()
	{
		DecreaseFoodStock();
		numTimesFoodEaten++;
		if(numTimesFoodEaten == maxFoodBeforeMorph)
		{
			morph = true;
		}
	}
	
	int AntLarva::GetFood() const
	{
		return food;
	}
	
	void AntLarva::IncreaseFoodStock()
	{
		food++;
	}

	void AntLarva::Run()
	{
	}
}