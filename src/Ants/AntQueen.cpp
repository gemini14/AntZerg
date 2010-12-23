#include "AntQueen.h"

#include <iostream>
namespace AntZerg
{
	static void Register(lua_State *luaState);

	AntQueen::AntQueen(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, false, lua, configFile, actionScriptFile, x, y), numLarvaeProduced(0)
	{
		using namespace luabind;

		auto confTable = lua->GetObject("QueenConf");
		assert(confTable.is_valid() && type(confTable) == LUA_TTABLE);
		SetScalingFactor(object_cast<float>(confTable["scale"]));
	}

	AntQueen::~AntQueen()
	{
	}

	void AntQueen::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
			numLarvaeProduced++;
			numLarvaeAvailable++;
		}
	}

	void AntQueen::ExtractLarvae()
	{
		if(numLarvaeAvailable)
		{
			numLarvaeAvailable--;
		}
	}

	int AntQueen::GetMaxLarvaeProduced() const
	{
		return numLarvaeProduced;
	}

	int AntQueen::GetNumAvailLarvae() const
	{
		return numLarvaeAvailable;
	}

	void AntQueen::RegisterLua(lua_State* luaState)
	{
		Register(luaState);
	}

	void AntQueen::Run()
	{
		lua->GetObject("QueenRun")(GetID());
	}

	void Register(lua_State *luaState)
	{
		using namespace luabind;

		module(luaState, "AntZerg")
			[
				class_<AntQueen, Ant>("AntQueen")
				.def("Eat", &AntQueen::Eat)
				.def("ExtractLarvae", &AntQueen::ExtractLarvae)
				.def("GetMaxLarvaeProduced", &AntQueen::GetMaxLarvaeProduced)
				.def("GetNumAvailableLarvae", &AntQueen::GetNumAvailLarvae)
			];
	}
}