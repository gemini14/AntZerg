#ifndef ANTLARVA_H
#define ANYLARVA_H


#include "Ant.h"


namespace AntZerg
{

	class AntLarva : public Ant
	{
	private:
		
		int numTimesFoodEaten;

		int maxFoodBeforeMorph;
		bool morph;
		
	public:

		AntLarva(const int ID, std::shared_ptr<LuaManager> lua, const std::string& configFile, 
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntLarva();
		
		bool CanMorph() const;
		virtual int Eat();
		int GetMorphFoodLimit() const;
		static luabind::scope RegisterLua();
		virtual void Run(const double dt);
	};

}

#endif