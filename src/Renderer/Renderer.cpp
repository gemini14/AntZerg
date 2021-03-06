#include "Renderer.h"


#include <utility>


namespace AntZerg
{
#ifndef _WIN32
#define nullptr NULL
#endif

	const int POLY_COUNT = 8;

	Renderer::AntInfo::AntInfo(const AntType type, const float x, const float y,
		irr::scene::ISceneNode *node)
		: type(type), x(x), y(y), node(node)
	{
	}

	Renderer::AntInfo::~AntInfo()
	{
		node->remove();
	}

	Renderer::FungusPlotInfo::FungusPlotInfo(const float x, const float y,
		irr::scene::ISceneNode *node)
		: x(x), y(y), node(node)
	{
	}

	Renderer::FungusPlotInfo::~FungusPlotInfo()
	{
		node->remove();
	}

	Renderer::Renderer(std::shared_ptr<AppManager> app, std::shared_ptr<LuaManager> lua)
		: app(app), lua(lua), warehouseNode(nullptr), fungusPlotTexture(nullptr)
	{
		using namespace luabind;
		using namespace irr;

		auto fetchInfo = [&](const std::string& antType)
		{
			using namespace luabind;

			if(antDisplayInfoTable.find(GetAntTypeFromString(antType)) != antDisplayInfoTable.end())
			{
				return;
			}

			auto texTable = lua->GetObject("AntTextures");
			auto scaleTable = lua->GetObject("AntScales");

			assert(texTable.is_valid() && type(texTable) == LUA_TTABLE);
			assert(scaleTable.is_valid() && type(scaleTable) == LUA_TTABLE);

			std::pair<AntType, std::shared_ptr<DisplayInfo>> pairInfo;
			bool validAnt = true;

			if(antType == "queen")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver,
					object_cast<float>(scaleTable["queen"]),
					object_cast<std::string>(texTable["queen"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = QUEEN;
			}
			else if(antType == "larva")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver,
					object_cast<float>(scaleTable["larva"]),
					object_cast<std::string>(texTable["larva"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = LARVA;
			}
			else if(antType == "nurse")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver,
					object_cast<float>(scaleTable["nurse"]),
					object_cast<std::string>(texTable["nurse"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = NURSE;
			}
			else if(antType == "worker")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver,
					object_cast<float>(scaleTable["worker"]),
					object_cast<std::string>(texTable["worker"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = WORKER;
			}
			else
			{
				validAnt = false;
			}


			if(validAnt)
			{
				antDisplayInfoTable.insert(pairInfo);
			}
		};

		lua->LoadScript("scripts/conf/antTextures.lua");
		lua->LoadScript("scripts/conf/antScales.lua");
		lua->LoadScript("scripts/conf/supportItemTextures.lua");

		fetchInfo("queen");
		fetchInfo("larva");
		fetchInfo("nurse");
		fetchInfo("worker");

		auto table = lua->GetObject("SupportTextures");
		assert(table.is_valid() && type(table) == LUA_TTABLE);
		std::string texFile = object_cast<std::string>(table["fungus"]);
		fungusPlotTexture = app->driver->getTexture(texFile.c_str());

		module(lua->GetLuaState())
			[
				class_<Renderer>("Renderer")
				.def("AddAnt", &Renderer::AddAnt)
				.def("AddFungusPlot", &Renderer::AddFungusPlot)
				.def("AddWarehouse", &Renderer::AddWarehouse)
				.def("RemoveAnt", &Renderer::RemoveAnt)
				.def("RemoveFungusPlot", &Renderer::RemoveFungusPlot)
				.def("UpdateAnt", &Renderer::UpdateAnt)
			];
		luabind::globals(lua->GetLuaState())["renderer"] = this;

		app->smgr->addCameraSceneNode(0, core::vector3df(0,30,0), core::vector3df(0,0,0));
	}

	Renderer::~Renderer()
	{
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			delete iter->second;
		}
		for(auto iter = plotTable.begin(); iter != plotTable.end(); ++iter)
		{
			delete iter->second;
		}
		warehouseNode->remove();
	}

	bool Renderer::IsIDPresent(const int ID)
	{
		return antLookupTable.find(ID) != antLookupTable.end();
	}

	Renderer::AntType Renderer::GetAntTypeFromString(const std::string& type)
	{
		AntType ant = BAD;

		if(type == "queen")
		{
			ant = QUEEN;
		}
		else if(type == "larva")
		{
			ant = LARVA;
		}
		else if(type == "worker")
		{
			ant = WORKER;
		}
		else if(type == "nurse")
		{
			ant = NURSE;
		}
		else if(type == "warrior")
		{
			ant = WARRIOR;
		}

		return ant;
	}

	void Renderer::AddAnt(const int ID, const std::string& type, const float x, const float y)
	{
		using namespace irr;

		if(!IsIDPresent(ID))
		{
			auto antType = GetAntTypeFromString(type);
			if(antType == BAD)
			{
				return;
			}

			float scale = antDisplayInfoTable[antType]->GetDisplayScale();
			antLookupTable[ID] = new AntInfo(antType, x, y,
				app->smgr->addSphereSceneNode(0.5f, POLY_COUNT, 0, -1, core::vector3df(x, 0, y), core::vector3df(0,0,0),
				core::vector3df(scale, scale, scale)));
			antLookupTable[ID]->node->setMaterialTexture(0, antDisplayInfoTable[antType]->GetTexture());
			antLookupTable[ID]->node->setMaterialFlag(video::EMF_LIGHTING, false);
		}
	}

	void Renderer::AddFungusPlot(const int ID, const float x, const float y)
	{
		using namespace luabind;
		using namespace irr;

		if(ID != -1 && (plotTable.find(ID) == plotTable.end()))
		{
			plotTable[ID] = new FungusPlotInfo(x, y,
				app->smgr->addSphereSceneNode(0.5f, POLY_COUNT, 0, -1, core::vector3df(x, 0, y)));
			plotTable[ID]->node->setMaterialTexture(0, fungusPlotTexture);
			plotTable[ID]->node->setMaterialFlag(video::EMF_LIGHTING, false);
		}
	}

	void Renderer::AddWarehouse(const float x, const float y)
	{
		using namespace luabind;
		using namespace irr;

		if(!warehouseNode)
		{
			warehouseNode = app->smgr->addSphereSceneNode(0.5f, POLY_COUNT, 0, -1, core::vector3df(x, 0, y));

			auto table = lua->GetObject("SupportTextures");
			assert(table.is_valid() && type(table) == LUA_TTABLE);
			std::string texFile = object_cast<std::string>(table["warehouse"]);
			auto tex = app->driver->getTexture(texFile.c_str());

			warehouseNode->setMaterialTexture(0, tex);
			warehouseNode->setMaterialFlag(video::EMF_LIGHTING, false);
		}
	}

	void Renderer::DrawAll()
	{
	}

	void Renderer::RemoveAnt(const int ID)
	{
		if(IsIDPresent(ID))
		{
			delete antLookupTable[ID];
			antLookupTable.erase(ID);
		}
	}

	void Renderer::RemoveFungusPlot(const int ID)
	{
		auto iter = plotTable.find(ID);
		if(iter != plotTable.end())
		{
			delete plotTable[ID];
			plotTable.erase(ID);
		}
	}

	void Renderer::UpdateAnt(const int ID, const float x, const float y)
	{
		if(IsIDPresent(ID))
		{
			antLookupTable[ID]->x = x;
			antLookupTable[ID]->y = y;

			antLookupTable[ID]->node->setPosition(irr::core::vector3df(x, 0, y));
		}
	}
}
