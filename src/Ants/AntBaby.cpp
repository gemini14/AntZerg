#include "AntBaby.h"


namespace AntZerg
{

	AntBaby::AntBaby(const std::string& configFile, const std::string& actionScriptFile, const float scalingFactor, 
			const float x, const float y)
			: Ant(configFile, actionScriptFile, scalingFactor, x, y)
	{
	}

	AntBaby::~AntBaby()
	{
	}

	void AntBaby::Run()
	{
	}
}