#include "AntFactory.h"


#include <algorithm>

#include <boost/foreach.hpp>
#include <luabind/iterator_policy.hpp>

#include "../FungusPlot/FungusPlot.h"
#include "Ant.h"
#include "AntLarva.h"
#include "AntQueen.h"
#include "AntNurse.h"
#include "AntWarehouse.h"
#include "AntWorker.h"


namespace AntZerg
{
	bool AntFactory::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	AntFactory::AntFactory(std::shared_ptr<LuaManager> lua) 
		: antID_counter(0), plotID_counter(0), numAntsAlive(0), numAntsDead(0), maxAntsAlive(0),
		lua(lua), warehouse(nullptr), queenID(-1)
	{
		using namespace luabind;
		module(lua->GetLuaState())
			[
				Ant::RegisterLua(),
				AntLarva::RegisterLua(),
				AntQueen::RegisterLua(),
				AntNurse::RegisterLua(),
				AntWorker::RegisterLua(),
				AntWarehouse::RegisterLua(),
				FungusPlot::RegisterLua(),
				class_<AntFactory>("AntFactory")
				.def("CreateAnt", &AntFactory::CreateAnt)
				.def("GetAntByID", &AntFactory::GetAntByID)
				.def("GetPlotByID", &AntFactory::GetPlotByID)
				.def("RemoveAnt", &AntFactory::RemoveAnt)
				.def("CreateWarehouse", &AntFactory::CreateWarehouse)
				.def("GetWarehouse", &AntFactory::GetWarehouse)
				.def("GetQueen", &AntFactory::GetQueen)
				.def("LarvaNeedsFood", &AntFactory::LarvaNeedsFood)
				.def("CreateFungusPlot", &AntFactory::CreateFungusPlot)
				.def("RemoveFungusPlot", &AntFactory::RemoveFungusPlot)
			];
		luabind::globals(lua->GetLuaState())["factory"] = this;
	}

	AntFactory::~AntFactory()
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			delete iter->second;
		}
	}

	int AntFactory::CreateAnt(const std::string& antType, const float x, const float y)
	{
		Ant *temp = nullptr;

		if(antType == "queen")
		{
			if(queenID == -1)
			{
				temp = new AntQueen(++antID_counter, lua, "scripts/conf/queenConf.lua", "scripts/actions/queen.lua", x, y);
				queenID = antID_counter;
			}			
		}
		else if(antType == "larva")
		{
			temp = new AntLarva(++antID_counter, lua, "scripts/conf/larvaConf.lua", "scripts/actions/larva.lua", x, y);
			larvaList.push_back(antID_counter);
		}
		else if(antType == "worker")
		{
			temp = new AntWorker(++antID_counter, lua, "scripts/conf/workerConf.lua", "scripts/actions/worker.lua", x, y);
		}
		else if(antType == "nurse")
		{
			temp = new AntNurse(++antID_counter, lua, "scripts/conf/nurseConf.lua", "scripts/actions/nurse.lua", x, y);
		}
		else if(antType == "warrior")
		{
		}
		else
		{
			temp = nullptr;
		}

		if(temp)
		{
			antLookupTable[antID_counter] = temp;
			numAntsAlive++;
			maxAntsAlive++;
		}
		return temp ? antID_counter : -1;
	}

	int AntFactory::CreateFungusPlot(const float x, const float y)
	{
		bool spotTaken = false;
		for(auto iter = fungusPlots.begin(); iter != fungusPlots.end(); ++iter)
		{
			if(iter->second->GetX() == x && iter->second->GetY() == y)
			{
				spotTaken = true;
				break;
			}
		}

		if(!spotTaken)
		{
			fungusPlots[++plotID_counter] = new FungusPlot(x, y);
			return plotID_counter;
		}
		return -1;
	}

	void AntFactory::CreateWarehouse(const float x, const float y)
	{
		if(!warehouse)
		{
			warehouse = new AntWarehouse(x, y);
		}
	}

	Ant* AntFactory::GetAntByID(const int ID)
	{
		// this check is done to ensure that we won't end up inserting a new value if the ID doesn't exist
		if(IsIDPresent(ID))
		{
			return antLookupTable[ID];
		}

		return nullptr;
	}

	FungusPlot* AntFactory::GetPlotByID(const int ID)
	{
		auto iter = fungusPlots.find(ID);
		if(iter != fungusPlots.end())
		{
			return fungusPlots[ID];
		}

		return nullptr;
	}

	Ant* AntFactory::GetQueen()
	{
		return GetAntByID(queenID);
	}

	AntWarehouse* AntFactory::GetWarehouse() const
	{
		return warehouse;
	}

	int AntFactory::LarvaNeedsFood(const int nurseID)
	{
		if(larvaList.size() == 0)
		{
			return -1;
		}

		for(auto iter = larvaList.begin(); iter != larvaList.end(); ++iter)
		{
			auto ant = dynamic_cast<AntLarva*>(GetAntByID((*iter)));
			if(ant->GetNurse() == nurseID)
			{
				if(ant->GetFood() + ant->GetNumTimesEaten() < ant->GetMorphFoodLimit())
				{
					return (*iter);
				}
			}
		}

		return -1;
	}

	void AntFactory::RemoveAnt(const int ID)
	{
		if(IsIDPresent(ID))
		{
			deletionList.push_back(ID);
			numAntsDead++;
			numAntsAlive--;

			auto iter = std::find(larvaList.begin(), larvaList.end(), ID);
			if(iter != larvaList.end())
			{
				larvaList.erase(iter);
			}
		}
	}

	void AntFactory::RemoveFungusPlot(const int ID)
	{
		auto iter = fungusPlots.find(ID);
		if(iter != fungusPlots.end())
		{
			delete iter->second;
			fungusPlots.erase(iter);
		}
	}

	void AntFactory::RunAll(const double dt)
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			iter->second->Run(dt);
		}

		for(auto iter = deletionList.begin(); iter != deletionList.end(); ++iter)
		{
			antLookupTable.erase(*iter);
		}
	}
}
