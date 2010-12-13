#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const std::string& configFile, const std::string& actionScriptFile, const float scalingFactor, 
		const float x, const float y)
		: configFile(configFile), actionScript(actionScriptFile), displayScalingFactor(scalingFactor), position(x, y)
	{
	}

	Ant::~Ant()
	{
	}

	float Ant::GetDispScaling() const
	{
		return displayScalingFactor;
	}

	irr::core::vector2df Ant::GetPosition() const
	{
		return position;
	}
}