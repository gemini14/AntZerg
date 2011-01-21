#include "AntNurse.h"


namespace AntZerg
{
	AntNurse::AntNurse(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, true, lua, configFile, actionScriptFile, x, y), larva_carry(false), targetID(-1)
	{
		using namespace luabind;

		auto confTable = lua->GetObject("NurseConf");
		assert(confTable.is_valid()  && type(confTable) == LUA_TTABLE);

		float move_speed = object_cast<float>(confTable["movement_speed"]);
		blackboard = NurseBlackboard(move_speed);
	}

	AntNurse::~AntNurse()
	{
	}

	int AntNurse::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
			return 1;
		}

		return 0;
	}

	int AntNurse::GetTargetID() const
	{
		return targetID;
	}

	bool AntNurse::IsCarryingLarva() const
	{
		return larva_carry;
	}

	luabind::scope AntNurse::RegisterLua()
	{
		using namespace luabind;
		return class_<AntNurse, Ant>("AntNurse")
			.def("IsCarryingLarva", &AntNurse::IsCarryingLarva)
			.def("SetLarvaCarry", &AntNurse::SetLarvaCarry)
			.def("WithdrawFood", &AntNurse::WithdrawFood)
			.def_readwrite("blackboard", &AntNurse::blackboard);
	}

	void AntNurse::Run(const double dt)
	{
		try
		{
			lua->CallFunction("NurseRun", GetID(), dt);
		}
		catch (luabind::error& e)
		{
			std::string error(lua_tostring(lua->GetLuaState(), -1));
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}

	void AntNurse::SetLarvaCarry(const bool carrying)
	{
		larva_carry = carrying;
	}

	int AntNurse::WithdrawFood(const int amount)
	{
		if(amount > 0)
		{
			int withdrawnFood = 0;
			while(withdrawnFood < amount)
			{
				int result = Eat();
				if(!result)
				{
					break;
				}
				withdrawnFood++;
			}
			return withdrawnFood;
		}

		return 0;
	}
}