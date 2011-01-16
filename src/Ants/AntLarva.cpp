#include "AntLarva.h"


namespace AntZerg
{
	AntLarva::AntLarva(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, false, lua, configFile, actionScriptFile, x, y), numTimesFoodEaten(0), morph(false)
	{
		using namespace luabind;

		auto confTable = lua->GetObject("LarvaConf");

		assert(confTable.is_valid() && type(confTable) == LUA_TTABLE);

		int foodMorph = object_cast<int>(confTable["maxFoodBeforeMorph"]);
		maxFoodBeforeMorph = foodMorph;
	}

	AntLarva::~AntLarva()
	{
	}

	bool AntLarva::CanMorph() const
	{
		return morph;
	}

	int AntLarva::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
			numTimesFoodEaten++;
			if(numTimesFoodEaten == maxFoodBeforeMorph)
			{
				morph = true;
			}
			return 1;
		}

		return 0;
	}	

	int AntLarva::GetMorphFoodLimit() const
	{
		return maxFoodBeforeMorph;
	}

	luabind::scope AntLarva::RegisterLua()
	{
		using namespace luabind;
		return class_<AntLarva, Ant>("AntLarva")
			.def("CanMorph", &AntLarva::CanMorph)
			.def("Eat", &AntLarva::Eat);
	}

	void AntLarva::Run(const double dt)
	{
		try
		{
			lua->CallFunction("LarvaRun", GetID(), dt);
		}
		catch (luabind::error& e)
		{
			std::string error = lua_tostring(lua->GetLuaState(), -1);
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}
}