#include "Ant.h"


namespace AntZerg
{
	Ant::Ant(const int ID, bool canMove, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: movementEnabled(canMove), lua(lua), position(x, y), configFile(configFile), 
		actionScript(actionScriptFile), food(0), ID(ID)
	{
		lua->LoadScript(configFile);
		lua->LoadScript(actionScriptFile);
	}

	Ant::~Ant()
	{
	}

	void Ant::AddFood(const int foodIncome)
	{
		if(foodIncome)
		{
			//std::cout << "Ant had " << food << " food, now has ";
			food += foodIncome;
			//std::cout << food << " food (income: " << foodIncome << ").\n";
		}
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

	void Ant::PositionChange(const float delta_x, const float delta_y)
	{
		position.X += delta_x;
		position.Y += delta_y;
	}

	luabind::scope Ant::RegisterLua()
	{
		using namespace luabind;
		return class_<Ant>("Ant")
			.def("AddFood", &Ant::AddFood)
			.def("CanMove", &Ant::CanMove)
			.def("Eat", &Ant::Eat)
			.def("GetFood", &Ant::GetFood)
			.def("GetID", &Ant::GetID)
			.def("GetX", &Ant::GetX)
			.def("GetY", &Ant::GetY)
			.def("PositionChange", &Ant::PositionChange);
	}
}
