#include "AntQueen.h"


namespace AntZerg
{
	AntQueen::AntQueen(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, false, lua, configFile, actionScriptFile, x, y), numLarvaeProduced(0), numLarvaeAvailable(0)
	{
	}

	AntQueen::~AntQueen()
	{
	}

	void AntQueen::CreateLarva()
	{
		if(GetFood() > 0)
		{
			numLarvaeProduced++;
			numLarvaeAvailable++;
		}
	}

	int AntQueen::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
			return 1;
		}
		
		return 0;
	}

	int AntQueen::ExtractLarvae()
	{
		if(numLarvaeAvailable)
		{
			numLarvaeAvailable--;
			return 1;
		}

		return 0;
	}

	int AntQueen::GetMaxLarvaeProduced() const
	{
		return numLarvaeProduced;
	}

	int AntQueen::GetNumAvailLarvae() const
	{
		return numLarvaeAvailable;
	}

	luabind::scope AntQueen::RegisterLua()
	{
		using namespace luabind;
		return class_<AntQueen, Ant>("AntQueen")
			.def("Eat", &AntQueen::Eat)
			.def("ExtractLarvae", &AntQueen::ExtractLarvae)
			.def("GetMaxLarvaeProduced", &AntQueen::GetMaxLarvaeProduced)
			.def("GetNumAvailLarvae", &AntQueen::GetNumAvailLarvae)
			.def("CreateLarva", &AntQueen::CreateLarva);
	}

	void AntQueen::Run(const double dt)
	{
		try
		{
			lua->CallFunction("QueenRun", GetID(), dt);
		}
		catch (luabind::error& e)
		{
			std::string error = lua_tostring(lua->GetLuaState(), -1);
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}
}