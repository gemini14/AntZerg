#ifndef LUAMANAGER_H
#define LUAMANAGER_H


#include <boost/noncopyable.hpp>
extern "C"
{
#include <lua.h>
}
#include <luabind/luabind.hpp>


namespace AntZerg
{

	class LuaManager: boost::noncopyable
	{
	private:

		lua_State *luaState;

	public:

		LuaManager();
		~LuaManager();

		lua_State* const GetLuaState();

	};

}

#endif
