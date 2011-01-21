#ifndef ANTWORKER_H
#define ANTWORKER_H


#include "Ant.h"
#include "Blackboards/WorkerBlackboard.h"


namespace AntZerg
{
	class AntWorker : public Ant
	{
	private:

	public:

		WorkerBlackboard blackboard;

		AntWorker(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile,
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntWorker();

		virtual int Eat();
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
		inline int WithdrawFood(const int amount);
	};
}

#endif