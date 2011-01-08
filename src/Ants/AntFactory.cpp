#include "AntFactory.h"


#include <boost/foreach.hpp>

#include "Ant.h"
#include "AntLarva.h"
#include "AntQueen.h"


namespace AntZerg
{
	bool AntFactory::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	AntFactory::AntFactory() 
		: ID_counter(0), numAntsAlive(0), numAntsDead(0), maxAntsAlive(0), lua(std::make_shared<LuaManager>())
	{
		using namespace luabind;
		module(lua->GetLuaState())
			[
				Ant::RegisterLua(),
				AntLarva::RegisterLua(),
				AntQueen::RegisterLua(),
				class_<AntFactory>("AntFactory")
					.def("CreateAnt", &AntFactory::CreateAnt)
					.def("GetAntByID", &AntFactory::GetAntByID)
					.def("RemoveAntByID", &AntFactory::RemoveAntByID)
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
			temp = new AntQueen(++ID_counter, lua, "scripts/conf/queen.lua", "scripts/actions/queen.lua", x, y);
		}
		else if(antType == "larva")
		{
			temp = new AntLarva(++ID_counter, lua, "scripts/conf/larva.lua", "scripts/actions/larva.lua", x, y);
		}
		else if(antType == "worker")
		{
		}
		else if(antType == "nurse")
		{
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

	Ant* AntFactory::GetAntByID(const int ID)
	{
		// this check is done to ensure that we won't end up inserting a new value if the ID doesn't exist
		if(IsIDPresent(ID))
		{
			return antLookupTable[ID];
		}

		return nullptr;
	}

	void AntFactory::RemoveAntByID(const int ID)
	{
		if(IsIDPresent(ID))
		{
			antLookupTable.erase(ID);
			numAntsDead++;
			numAntsAlive--;
		}
	}

	void AntFactory::RenderUpdateAll()
	{
	}

	void AntFactory::RunAll()
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			iter->second->Run();
		}
	}
}
