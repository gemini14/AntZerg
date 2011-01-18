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

		void CreateLarva();
		virtual int Eat();
		inline int ExtractLarvae();
		int GetMaxLarvaeProduced() const;
		inline int GetNumAvailLarvae() const;
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
	};
}

#endif