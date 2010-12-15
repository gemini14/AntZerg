#include "AntQueen.h"


namespace AntZerg
{
		AntQueen::AntQueen(const int ID, const std::string& configFile, const std::string& actionScriptFile,
			const float x, const float y)
			: Ant(ID, false, configFile, actionScriptFile, x, y), numLarvaeProduced(0)
		{
		}
		
		AntQueen::~AntQueen()
		{
		}

		void AntQueen::Eat()
		{
			if(GetFood() > 0)
			{
				DecreaseFoodStock();
				numLarvaeProduced++;
				numLarvaeAvailable++;
			}
		}
		
		void AntQueen::Run()
		{
			// TODO: implement run for queen
		}
}