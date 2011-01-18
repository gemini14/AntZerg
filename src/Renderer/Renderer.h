#ifndef RENDERER_H
#define RENDERER_H


#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>
#include <irrlicht.h>

#include "../Ants/Ant.h"
#include "../Ants/AntWarehouse.h"
#include "../AppManager.h"
#include "../Lua/LuaManager.h"
#include "DisplayInfo.h"


namespace AntZerg
{
	class Renderer : boost::noncopyable
	{
	private:

		enum AntType { BAD, QUEEN, LARVA, WORKER, NURSE, WARRIOR };
		
		class AntInfo : boost::noncopyable
		{
		public:

			AntType type;
			float x;
			float y;
			irr::scene::ISceneNode *node;

			AntInfo(const AntType type, const float x, const float y, irr::scene::ISceneNode *node);
			~AntInfo();
		};

		class FungusPlotInfo : boost::noncopyable
		{
		public:

			float x;
			float y;
			irr::scene::ISceneNode *node;

			FungusPlotInfo(const float x, const float y, irr::scene::ISceneNode *node);
			~FungusPlotInfo();
		};

		typedef std::map<AntType, std::shared_ptr<DisplayInfo>> AntDisplayMap;
		typedef std::unordered_map<int, AntInfo*> AntHash;
		typedef std::unordered_map<int, FungusPlotInfo*> FungusPlotHash;

		AntDisplayMap antDisplayInfoTable;
		AntHash antLookupTable;
		FungusPlotHash plotTable;
		std::shared_ptr<AppManager> app;
		std::shared_ptr<LuaManager> lua;
		irr::scene::ISceneNode *warehouseNode;
		irr::video::ITexture *fungusPlotTexture;

		bool IsIDPresent(const int ID);
		AntType GetAntTypeFromString(const std::string& type);

	public:

		Renderer(std::shared_ptr<AppManager> app, std::shared_ptr<LuaManager> lua);
		~Renderer();

		void AddAnt(const int ID, const std::string& type, const float x, const float y);
		void AddFungusPlot(const int ID, const float x, const float y);
		void AddWarehouse(const float x, const float y);
		void DrawAll();
		void RemoveAnt(const int ID);
		void RemoveFungusPlot(const int ID);
		void UpdateAnt(const int ID, const float x, const float y);
	};
}

#endif