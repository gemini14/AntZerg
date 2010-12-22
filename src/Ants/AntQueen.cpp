#include "AntQueen.h"


namespace AntZerg
{
	static void Register(lua_State *luaState);

	AntQueen::AntQueen(const int ID, const std::string& configFile, const std::string& actionScriptFile,
		const float x, const float y)
		: Ant(ID, false, configFile, actionScriptFile, x, y), numLarvaeProduced(0)
	{
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
		// TODO: implement run for queen
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