#include "LuaManager.h"


#include <iostream>


namespace AntZerg
{

	LuaManager::LuaManager()
	{
		luaState = lua_open();
		luabind::open(luaState);
		luaL_openlibs(luaState);
	}

	LuaManager::~LuaManager()
	{
		lua_close(luaState);
	}

	lua_State* const LuaManager::GetLuaState() const
	{
		return luaState;
	}

	bool LuaManager::LoadScript(const std::string& filename)
	{
		using namespace luabind;

		int result = luaL_dofile(luaState, filename.c_str());
		if(result)
		{
#ifdef _DEBUG
			std::cout << "Error loading lua script.  Error code: " << result << std::endl;
#endif
			return false;
		}
		
		return true;
	}

	luabind::object LuaManager::GetObject(const std::string& objName)
	{
		// must check if valid in caller
		return luabind::globals(luaState)[objName.c_str()];
	}

}
