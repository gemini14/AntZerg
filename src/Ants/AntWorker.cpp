#include "AntWorker.h"


namespace AntZerg
{
	AntWorker::AntWorker(const int ID, std::shared_ptr<LuaManager> lua, 
		const std::string& configFile,const std::string& actionScriptFile, const float x, const float y)
		: Ant(ID, true, lua, configFile, actionScriptFile, x, y)
	{
		using namespace luabind;

		auto confTable = lua->GetObject("WorkerConf");
		assert(confTable.is_valid()  && type(confTable) == LUA_TTABLE);

		float move_speed = object_cast<float>(confTable["movement_speed"]);
		blackboard = WorkerBlackboard(move_speed);
	}

	AntWorker::~AntWorker()
	{
	}

	int AntWorker::Eat()
	{
		if(GetFood() > 0)
		{
			DecreaseFoodStock();
			return 1;
		}
		
		return 0;
	}

	luabind::scope AntWorker::RegisterLua()
	{
		using namespace luabind;
		return class_<AntWorker, Ant>("AntWorker")
			.def("Eat", &AntWorker::Eat)
			.def("WithdrawFood", &AntWorker::WithdrawFood)
			.def_readwrite("blackboard", &AntWorker::blackboard);
	}

	void AntWorker::Run(const double dt)
	{
		try
		{
			lua->CallFunction("WorkerRun", GetID(), dt);
		}
		catch (luabind::error& e)
		{
			std::string error(lua_tostring(lua->GetLuaState(), -1));
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}

	int AntWorker::WithdrawFood(const int amount)
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