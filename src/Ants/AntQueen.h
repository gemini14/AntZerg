#ifndef ANTQUEEN_H
#define ANTQUEEN_H


#include "Ant.h"


namespace AntZerg
{
	class AntQueen : public Ant
	{
	private:

		int numLarvaeProduced;
		int numLarvaeAvailable;

	public:

		AntQueen(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntQueen();

		virtual int Eat();
		void ExtractLarvae();
		int GetMaxLarvaeProduced() const;
		int GetNumAvailLarvae() const;
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
	};
}

#endif