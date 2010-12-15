#ifndef ANTLARVA_H
#define ANYLARVA_H


#include "Ant.h"


namespace AntZerg
{

	class AntLarva : public Ant
	{
	private:

		bool movementEnabled;
		int food;
		int numTimesFoodEaten;
		
		// this is set to desired value in Lua script
		int maxFoodBeforeMorph;
		bool morph;
		
	public:

		AntLarva(const std::string& configFile, const std::string& actionScriptFile, const float x, const float y);
		virtual ~AntLarva();

		void DecreaseFoodStock();
		virtual void Eat();
		int GetFood() const;
		void IncreaseFoodStock();

		virtual void Run();
	};

}

#endif