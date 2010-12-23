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

		AntLarva(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntLarva();
		
		virtual void Eat();
		static void RegisterLua(lua_State* luaState);
		virtual void Run();
	};

}

#endif