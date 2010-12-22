#include "LuaManager.h"

namespace AntZerg
{

	LuaManager::LuaManager()
	{
		luaState = lua_open();
		luabind::open(luaState);
	}

	LuaManager::~LuaManager()
	{
		lua_close(luaState);
	}

}
