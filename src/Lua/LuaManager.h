#ifndef LUAMANAGER_H
#define LUAMANAGER_H


#include <boost/noncopyable.hpp>
#include <luabind/luabind.hpp>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


namespace AntZerg
{

	class LuaManager: boost::noncopyable
	{
	private:

		lua_State *luaState;

	public:

		LuaManager();
		~LuaManager();

		void CallFunction(const std::string& functionName);
		void CallFunction(const std::string& functionName, const int ID, const double dt);
		lua_State* const GetLuaState() const;
		luabind::object GetObject(const std::string& objName);
		bool LoadScript(const std::string& filename);

	};

}

#endif
