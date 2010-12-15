#include "AntLarva.h"


namespace AntZerg
{
	AntLarva::AntLarva(const int ID, const std::string& configFile, const std::string& actionScriptFile, 
		const float x, const float y)
		: Ant(ID, false, configFile, actionScriptFile, x, y), numTimesFoodEaten(0), maxFoodBeforeMorph(1),
		morph(false)
	{
	}

	AntLarva::~AntLarva()
	{
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

	void AntLarva::Run()
	{
		// TODO: implement larva run
	}
}