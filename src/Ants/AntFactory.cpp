#include "AntFactory.h"


#include <boost/foreach.hpp>
#include <luabind/iterator_policy.hpp>

#include "Ant.h"
#include "AntLarva.h"
#include "AntQueen.h"
#include "AntNurse.h"
#include "AntWarehouse.h"


namespace AntZerg
{
	bool AntFactory::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	AntFactory::AntFactory(std::shared_ptr<LuaManager> lua) 
		: ID_counter(0), numAntsAlive(0), numAntsDead(0), maxAntsAlive(0), lua(lua),
		warehouse(nullptr), queenID(-1)
	{
		using namespace luabind;
		module(lua->GetLuaState())
			[
				Ant::RegisterLua(),
				AntLarva::RegisterLua(),
				AntQueen::RegisterLua(),
				AntNurse::RegisterLua(),
				AntWarehouse::RegisterLua(),
				class_<AntFactory>("AntFactory")
					.def("CreateAnt", &AntFactory::CreateAnt)
					.def("GetAntByID", &AntFactory::GetAntByID)
					.def("RemoveAnt", &AntFactory::RemoveAnt)
					.def("CreateWarehouse", &AntFactory::CreateWarehouse)
					.def("GetWarehouse", &AntFactory::GetWarehouse)
					.def("GetQueen", &AntFactory::GetQueen)
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
				temp = new AntQueen(++ID_counter, lua, "scripts/conf/queenConf.lua", "scripts/actions/queen.lua", x, y);
				queenID = ID_counter;
			}			
		}
		else if(antType == "larva")
		{
			temp = new AntLarva(++ID_counter, lua, "scripts/conf/larvaConf.lua", "scripts/actions/larva.lua", x, y);
		}
		else if(antType == "worker")
		{
		}
		else if(antType == "nurse")
		{
			temp = new AntNurse(++ID_counter, lua, "scripts/conf/nurseConf.lua", "scripts/actions/nurse.lua", x, y);
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
			antLookupTable[ID_counter] = temp;
			numAntsAlive++;
			maxAntsAlive++;
		}
		return temp ? ID_counter : -1;
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

	Ant* AntFactory::GetQueen()
	{
		return GetAntByID(queenID);
	}
		
	AntWarehouse* AntFactory::GetWarehouse() const
	{
		return warehouse;
	}

	void AntFactory::RemoveAnt(const int ID)
	{
		if(IsIDPresent(ID))
		{
			antLookupTable.erase(ID);
			numAntsDead++;
			numAntsAlive--;
		}
	}

	void AntFactory::RunAll(const double dt)
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			iter->second->Run(dt);
		}
	}
}
