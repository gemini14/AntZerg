#ifndef ANTFACTORY_H
#define ANTFACTORY_H


#include <memory>
#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "../Lua/LuaManager.h"


namespace AntZerg
{
	class Ant;

	class AntFactory : boost::noncopyable
	{
		typedef std::unordered_map<int, Ant*> AntHash;
		AntHash antLookupTable;
		std::shared_ptr<LuaManager> lua;

		int ID_counter;
		int numAntsAlive;
		int numAntsDead;
		int maxAntsAlive;

		bool IsIDPresent(const int ID);

	public:

		AntFactory(std::shared_ptr<LuaManager> lua);
		~AntFactory();

		int CreateAnt(const std::string& antType, const float x, const float y);
		Ant* GetAntByID(const int ID);
		void RemoveAntByID(const int ID);
		void RenderUpdateAll();
		void RunAll();
	};

}

#endif
