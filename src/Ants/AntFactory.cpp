#include "AntFactory.h"


#include <unordered_map>

#include "Ant.h"
#include "AntLarva.h"
#include "AntQueen.h"


namespace AntZerg
{
	typedef std::unordered_map<int, Ant*> AntHash;
	AntHash antLookupTable;

	int ID_counter = 0;
	int numAntsAlive = 0;
	int numAntsDead = 0;
	int maxAntsAlive = 0;


	bool AntFactory::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	int AntFactory::CreateAnt(const std::string& antType, const float x, const float y)
	{
		Ant *temp = nullptr;

		if(antType == "queen")
		{
			temp = new AntQueen(++ID_counter, "scripts/conf/queen.lua", "scripts/actions/queen.lua", x, y);
		}
		else if(antType == "larva")
		{
			temp = new AntLarva(++ID_counter, "scripts/conf/larva.lua", "scripts/actions/larva.lua", x, y);
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
}
