#include "AntLarva.h"


namespace AntZerg
{
	AntLarva::AntLarva(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, false, lua, configFile, actionScriptFile, x, y), numTimesFoodEaten(0), maxFoodBeforeMorph(1),
		morph(false)
	{
		using namespace luabind;
		auto confTable = lua->GetObject("LarvaConf");
		assert(confTable.is_valid() && type(confTable) == LUA_TTABLE);
		SetScalingFactor(object_cast<float>(confTable["scale"]));
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

	luabind::scope AntLarva::RegisterLua()
	{
		using namespace luabind;
		return class_<AntLarva, Ant>("AntLarva")
				.def("Eat", &AntLarva::Eat);
	}

	void AntLarva::Run()
	{
		lua->GetObject("LarvaRun")(GetID());
	}
}