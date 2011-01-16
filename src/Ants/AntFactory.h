#ifndef ANTFACTORY_H
#define ANTFACTORY_H


#include <memory>
#include <list>
#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "../Lua/LuaManager.h"


namespace AntZerg
{
	class Ant;
	class AntWarehouse;

	class AntFactory : boost::noncopyable
	{
		typedef std::unordered_map<int, Ant*> AntHash;

		AntHash antLookupTable;
		std::shared_ptr<LuaManager> lua;
		AntWarehouse *warehouse;
		std::list<int> larvaList;

		int ID_counter;
		int numAntsAlive;
		int numAntsDead;
		int maxAntsAlive;
		int queenID;

		bool IsIDPresent(const int ID);

	public:

		AntFactory(std::shared_ptr<LuaManager> lua);
		~AntFactory();

		int CreateAnt(const std::string& antType, const float x, const float y);
		void CreateWarehouse(const float x, const float y);
		Ant* GetAntByID(const int ID);
		Ant* GetQueen();
		AntWarehouse* GetWarehouse() const;
		int LarvaNeedsFood();
		void RemoveAnt(const int ID);
		void RunAll(const double dt);
	};

}

#endif
