#ifndef ANTBABY_H
#define ANYBABY_H


#include "Ant.h"


namespace AntZerg
{

	class AntBaby : Ant
	{
	private:

		bool movementEnabled;

	public:

		AntBaby(const std::string& configFile, const std::string& actionScriptFile, const float scalingFactor, 
			const float x, const float y);
		virtual ~AntBaby();

		virtual void Run();
	};

}

#endif