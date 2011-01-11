#include "AntNurse.h"


namespace AntZerg
{
	AntNurse::AntNurse(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, false, lua, configFile, actionScriptFile, x, y), larvaID_carry(-1), targetID(-1)
	{
	}

	AntNurse::~AntNurse()
	{
	}

	void AntNurse::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
		}
	}

	int AntNurse::GetTargetID() const
	{
		return targetID;
	}

	bool AntNurse::IsCarryingLarva() const
	{
		if(larvaID_carry != -1)
		{
			return true;
		}
		return false;
	}

	luabind::scope AntNurse::RegisterLua()
	{
		using namespace luabind;
		return class_<AntNurse, Ant>("AntNurse")
			.def("IsCarryingLarva", &AntNurse::IsCarryingLarva)
			.def("SetLarvaID_Carry", &AntNurse::SetLarvaID_Carry);
	}

	void AntNurse::Run(const double dt)
	{
		try
		{
			lua->CallFunction("NurseRun", GetID(), dt);
		}
		catch (luabind::error& e)
		{
			std::string error = lua_tostring(lua->GetLuaState(), -1);
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}

	void AntNurse::SetLarvaID_Carry(const int ID)
	{
		larvaID_carry = ID;
	}
}