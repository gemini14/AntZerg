#ifndef LUAMANAGER_H
#define LUAMANAGER_H


#include <boost/noncopyable.hpp>
#include <luabind/luabind.hpp>
#include <luabind/lua_include.hpp>
extern "C"
{
#include <lualib.h>
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

		void CallFunction(const std::string& functionName, int ID);
		lua_State* const GetLuaState() const;
		luabind::object GetObject(const std::string& objName);
		bool LoadScript(const std::string& filename);

	};

}

#endif
