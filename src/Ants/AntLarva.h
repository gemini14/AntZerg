#ifndef ANTLARVA_H
#define ANYLARVA_H


#include "Ant.h"


namespace AntZerg
{

	class AntLarva : public Ant
	{
	private:
		
		int numTimesFoodEaten;

		// this is set to desired value in Lua script
		int maxFoodBeforeMorph;
		bool morph;
		
	public:

		AntLarva(const int ID, const std::string& configFile, const std::string& actionScriptFile, 
			const float x, const float y);
		virtual ~AntLarva();
		
		virtual void Eat();
		virtual void RegisterLua(lua_State* luaState);
		virtual void Run();
	};

}

#endif