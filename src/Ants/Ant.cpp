#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const int ID, bool canMove, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: movementEnabled(canMove), lua(lua), position(x, y), displayScalingFactor(1), configFile(configFile), 
		actionScript(actionScriptFile), food(0), ID(ID)
	{
		lua->LoadScript(configFile);
		lua->LoadScript(actionScriptFile);
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

	luabind::scope Ant::RegisterLua()
	{
		using namespace luabind;
		return class_<Ant>("Ant")
				.def("CanMove", &Ant::CanMove)
				.def("Eat", &Ant::Eat)
				.def("GetFood", &Ant::GetFood)
				.def("GetID", &Ant::GetID)
				.def("GetX", &Ant::GetX)
				.def("GetY", &Ant::GetY);
	}

	void Ant::SetScalingFactor(const float scale)
	{
		if(scale)
		{
			displayScalingFactor = scale;
		}
	}
}
