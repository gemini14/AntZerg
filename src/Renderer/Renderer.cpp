#include "Renderer.h"


#include <utility>


namespace AntZerg
{
	Renderer::AntInfo::AntInfo(const AntType type, const float x, const float y, 
		const float rotation, irr::scene::ISceneNode *node)
		: type(type), x(x), y(y), rotation(rotation), node(node), texture1(true)
	{
	}

	Renderer::AntInfo::~AntInfo()
	{
		node->remove();
	}

	Renderer::Renderer(std::shared_ptr<AppManager> app, std::shared_ptr<LuaManager> lua)
		: app(app), lua(lua)
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
					object_cast<std::string>(texTable["queen1"]),
					object_cast<std::string>(texTable["queen2"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = QUEEN;
			}
			else if(antType == "larva")
			{
				std::shared_ptr<DisplayInfo> dispInfo((new DisplayInfo(
					app->driver, 
					object_cast<float>(scaleTable["larva"]),
					object_cast<std::string>(texTable["larva1"]),
					object_cast<std::string>(texTable["larva2"]))));
				pairInfo.second = dispInfo;
				pairInfo.first = LARVA;
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

		fetchInfo("queen");
		fetchInfo("larva");

		module(lua->GetLuaState())
			[
				class_<Renderer>("Renderer")
				.def("AddAnt", &Renderer::AddAnt)
				.def("RemoveAnt", &Renderer::RemoveAnt)
				.def("UpdateAnt", &Renderer::UpdateAnt)
			];
		luabind::globals(lua->GetLuaState())["renderer"] = this;

		app->smgr->addCameraSceneNode(0, core::vector3df(3,25,0), core::vector3df(0,0,0));
	}

	Renderer::~Renderer()
	{
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

	void Renderer::AddAnt(const int ID, const std::string& type, const float x, const float y, const float rotation)
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
			antLookupTable[ID] = new AntInfo(antType, x, y, rotation, 
				app->smgr->addSphereSceneNode(0.5f, 16, 0, -1, core::vector3df(x, 0, y), core::vector3df(0,0,0),
				core::vector3df(scale, scale, scale)));
			antLookupTable[ID]->node->setMaterialTexture(0, antDisplayInfoTable[antType]->GetTexture1());
			antLookupTable[ID]->node->setMaterialFlag(video::EMF_LIGHTING, false);
		}
	}

	void Renderer::DrawAll()
	{
		static auto prevTime = app->device->getTimer()->getTime();
		auto dt = app->device->getTimer()->getTime() - prevTime;
		dt /= 1000;
		dt %= 2;
		for(auto iter = antLookupTable.begin(); iter != antLookupTable.end(); ++iter)
		{
			bool currentTexture = iter->second->texture1;
			bool updateTexture = currentTexture && dt;
			if(updateTexture != currentTexture)
			{
				iter->second->texture1 = !currentTexture;
				switch(currentTexture)
				{
				case true:
					iter->second->node->setMaterialTexture(0, antDisplayInfoTable[iter->second->type]->GetTexture2());
					break;
				case false:
					iter->second->node->setMaterialTexture(0, antDisplayInfoTable[iter->second->type]->GetTexture1());
				}
			}
		}
	}

	void Renderer::RemoveAnt(const int ID)
	{
		if(IsIDPresent(ID))
		{
			delete antLookupTable[ID];
			antLookupTable.erase(ID);
		}
	}

	void Renderer::UpdateAnt(const int ID, const float x, const float y, const float rotation)
	{
		if(IsIDPresent(ID))
		{
			antLookupTable[ID]->x = x;
			antLookupTable[ID]->y = y;
			antLookupTable[ID]->rotation = rotation;
		}
	}
}