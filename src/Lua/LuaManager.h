#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <boost/noncopyable.hpp>
#include <luabind/luabind.hpp>

namespace AntZerg
{

	class LuaManager: boost::noncopyable
	{
		public:

			LuaManager();
			~LuaManager();
	};

}

#endif
