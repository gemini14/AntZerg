#ifndef ANTFACTORY_H
#define ANTFACTORY_H


#include <list>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "../Lua/LuaManager.h"


namespace AntZerg
{
	class Ant;
	class AntWarehouse;
	class FungusPlot;

	class AntFactory : boost::noncopyable
	{
		class AntInfo
		{
		public:
			std::string type;
			float x;
			float y;
			AntInfo(const std::string& type, const float x, const float y);
			~AntInfo();
		};

		typedef std::unordered_map<int, Ant*> AntHash;
		typedef std::unordered_map<int, FungusPlot*> FungusPlotHash;
		typedef std::queue<AntInfo> AddQueue;
		typedef std::queue<int> DeleteQueue;

		AntHash antLookupTable;
		std::shared_ptr<LuaManager> lua;
		AntWarehouse *warehouse;
		std::list<int> larvaList;
		DeleteQueue queueDelete;
		AddQueue queueAdd;
		FungusPlotHash fungusPlots;

		int antID_counter;
		int plotID_counter;
		int numAntsAlive;
		int numAntsDead;
		int maxAntsAlive;
		int queenID;

		bool IsIDPresent(const int ID);

	public:

		AntFactory(std::shared_ptr<LuaManager> lua);
		~AntFactory();

		int CreateAnt(const std::string& antType, const float x, const float y);
		int CreateFungusPlot(const float x, const float y);
		void CreateWarehouse(const float x, const float y);
		Ant* GetAntByID(const int ID);
		FungusPlot* GetPlotByID(const int ID);
		Ant* GetQueen();
		AntWarehouse* GetWarehouse() const;
		int LarvaNeedsFood(const int nurseID);
		void QueueCreateAnt(const std::string& antType, const float x, const float y);
		void RemoveAnt(const int ID);
		void RemoveFungusPlot(const int ID);
		void RunAll(const double dt);
	};

}

#endif
