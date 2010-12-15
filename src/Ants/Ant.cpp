#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const std::string& configFile, const std::string& actionScriptFile, const float x, const float y)
		: position(x, y), displayScalingFactor(1), configFile(configFile), actionScript(actionScriptFile)
	{
	}

	Ant::~Ant()
	{
	}

	float Ant::GetDispScaling() const
	{
		return displayScalingFactor;
	}

	int Ant::GetID() const
	{
		return ID;
	}

	irr::core::vector2df Ant::GetPosition() const
	{
		return position;
	}

	float Ant::GetX() const
	{
		return position.X;
	}

	float Ant::GetY() const
	{
		return position.Y;
	}

	void Ant::SetScalingFactor(const float scale)
	{
		if(scale > 0)
		{
			displayScalingFactor = scale;
		}
	}
}
