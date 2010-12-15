#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const int ID, bool canMove, const std::string& configFile, const std::string& actionScriptFile, 
		const float x, const float y)
		: movementEnabled(canMove), position(x, y), displayScalingFactor(1), configFile(configFile), 
		actionScript(actionScriptFile), food(0), ID(ID)
	{
	}

	Ant::~Ant()
	{
	}

	bool Ant::CanMove() const
	{
		return movementEnabled;
	}

	void Ant::DecreaseFoodStock()
	{
		if(food)
		{
			food--;
		}
	}

	float Ant::GetDispScaling() const
	{
		return displayScalingFactor;
	}

	int Ant::GetFood() const
	{
		return food;
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

	void Ant::IncreaseFoodStock()
	{
		food++;
	}

	void Ant::SetScalingFactor(const float scale)
	{
		if(scale)
		{
			displayScalingFactor = scale;
		}
	}
}
