#ifndef RENDERER_H
#define RENDERER_H


#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "../Ants/Ant.h"
#include "../AppManager.h"
#include "../Lua/LuaManager.h"
#include "DisplayInfo.h"


namespace AntZerg
{
	enum AntType { QUEEN, LARVA, WORKER, NURSE, GUARD };

	class Renderer : boost::noncopyable
	{
	private:

		struct AntInfo
		{
			AntType type;
			int x;
			int y;
			float rotation;
		};

		typedef std::map<std::string, std::shared_ptr<DisplayInfo>> AntDisplayMap;
		typedef std::unordered_map<int, AntInfo> AntHash;

		AntDisplayMap antDisplayInfoTable;
		AntHash antLookupTable;
		std::shared_ptr<AppManager> app;
		std::shared_ptr<LuaManager> lua;

		bool IsIDPresent(const int ID);

	public:

		Renderer(std::shared_ptr<AppManager> app, std::shared_ptr<LuaManager> lua);
		~Renderer();

		void AddAnt(const int ID, const AntType type, const int x, const int y, const float rotation);
		void DrawAll();
		void RemoveAnt(const int ID);
	};
}

#endif