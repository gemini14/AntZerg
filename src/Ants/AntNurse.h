#ifndef ANTNURSE_H
#define ANTNURSE_H


#include "Ant.h"


namespace AntZerg
{
	class AntNurse : public Ant
	{
	private:

		int babyID_carry;
		int targetID;

	public:

		AntNurse(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
		const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntNurse();

		virtual void Eat();
		bool IsCarryingBaby() const;
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
		void SetBabyID_Carry(const int ID);
	};
}


#endif