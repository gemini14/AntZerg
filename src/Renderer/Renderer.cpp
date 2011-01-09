#include "Renderer.h"


#include <utility>


namespace AntZerg
{
	Renderer::Renderer(std::shared_ptr<AppManager> app, std::shared_ptr<LuaManager> lua)
		: app(app), lua(lua)
	{
		auto fetchInfo = [&](const std::string& antType)
		{
			using namespace luabind;

			if(antDisplayInfoTable.find(antType) != antDisplayInfoTable.end())
				return;

			auto texTable = lua->GetObject("AntTextures");
			auto scaleTable = lua->GetObject("AntScales");

			assert(texTable.is_valid() && type(texTable) == LUA_TTABLE);
			assert(scaleTable.is_valid() && type(scaleTable) == LUA_TTABLE);

			std::pair<std::string, std::shared_ptr<DisplayInfo>> pairInfo;
			bool validAnt = true;

			if(antType == "queen")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver, 
					object_cast<float>(scaleTable["queen"]),
					object_cast<std::string>(texTable["queen1"]),
					object_cast<std::string>(texTable["queen2"]))));
				pairInfo.second = dispInfo;
			}
			else if(antType == "larva")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver, 
					object_cast<float>(scaleTable["larva"]),
					object_cast<std::string>(texTable["larva1"]),
					object_cast<std::string>(texTable["larva2"]))));
				pairInfo.second = dispInfo;
			}
			else
			{
				validAnt = false;
			}


			if(validAnt)
			{
				pairInfo.first = antType;
				antDisplayInfoTable.insert(pairInfo);
			}
		};

		lua->LoadScript("scripts/conf/antTextures.lua");
		lua->LoadScript("scripts/conf/antScales.lua");

		fetchInfo("queen");
		fetchInfo("larva");
	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	void Renderer::AddAnt(const int ID, const AntType type, const int x, const int y, const float rotation)
	{
		if(!IsIDPresent(ID))
		{
			AntInfo ant;
			ant.type = type;
			ant.x = x;
			ant.y = y;
			ant.rotation = rotation;
			antLookupTable.insert(std::make_pair<int, AntInfo>(ID, ant));
		}
	}

	void Renderer::DrawAll()
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			//app->driver->dr
		}
	}

	void Renderer::RemoveAnt(const int ID)
	{
		if(IsIDPresent(ID))
		{
			antLookupTable.erase(ID);
		}
	}
}