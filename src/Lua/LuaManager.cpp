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

	void LuaManager::CallFunction(const std::string& functionName)
	{
		try
		{
			luabind::call_function<void>(luaState, functionName.c_str());
		}
		catch (luabind::error& e)
		{
			std::string error = lua_tostring( luaState, -1 );
			std::cout << "\n" << e.what() << "\n" << error << "\n";
		}
	}

	void LuaManager::CallFunction(const std::string& functionName, int ID, const double dt)
	{
		luabind::call_function<void>(luaState, functionName.c_str(), ID, dt);
	}

	lua_State* const LuaManager::GetLuaState() const
	{
		return luaState;
	}

	luabind::object LuaManager::GetObject(const std::string& objName)
	{
		// must check if valid in caller
		return luabind::globals(luaState)[objName.c_str()];
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
			std::string error = lua_tostring(luaState, -1);
			std::cout << "\n" << error << "\n";
			return false;
		}

		return true;
	}

}
