#ifndef ANT_H
#define ANT_H


#include <memory>
#include <string>

#include <boost/noncopyable.hpp>
#include <irrlicht.h>
#include <luabind/luabind.hpp>

#include "../Lua/LuaManager.h"


namespace AntZerg
{
	class Ant : boost::noncopyable
	{
	private:

		bool movementEnabled;

		// this could be changed to another vector implementation later if needed
		irr::core::vector2df position;	
		
		std::string configFile;
		std::string actionScript;

		int food;
		
		int ID;

	protected:

		std::shared_ptr<LuaManager> lua;

	public:

		Ant(const int ID, bool canMove, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~Ant();

		bool CanMove() const;
		void DecreaseFoodStock();
		virtual void Eat() = 0;
		int GetFood() const;
		int GetID() const;
		irr::core::vector2df GetPosition() const;
		float GetX() const;
		float GetY() const;
		void IncreaseFoodStock();
		static luabind::scope RegisterLua();
		
		virtual void Run() = 0;
	};
}

#endif
