#ifndef ANTNURSE_H
#define ANTNURSE_H


#include "Ant.h"


namespace AntZerg
{
	class AntNurse : public Ant
	{
	private:

		bool larva_carry;
		int targetID;

	public:

		AntNurse(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntNurse();

		virtual int Eat();
		inline int GetTargetID() const;
		inline bool IsCarryingLarva() const;
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
		inline void SetLarvaCarry(const bool carrying);
		inline int WithdrawFood(const int amount);
	};
}


#endif