#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const std::string& configFile, const std::string& actionScriptFile, const float scalingFactor,
		const float x, const float y)
		: position(x, y), displayScalingFactor(scalingFactor), configFile(configFile), actionScript(actionScriptFile)
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
