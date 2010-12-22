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

		AntQueen(const int ID, const std::string& configFile, const std::string& actionScriptFile,
			const float x, const float y);
		virtual ~AntQueen();

		virtual void Eat();
		void ExtractLarvae();
		int GetMaxLarvaeProduced() const;
		int GetNumAvailLarvae() const;
		virtual void RegisterLua(lua_State* luaState);
		virtual void Run();
	};
}

#endif