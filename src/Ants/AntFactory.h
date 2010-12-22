#ifndef ANTFACTORY_H
#define ANTFACTORY_H


#include <string>

#include <boost/noncopyable.hpp>


namespace AntZerg
{
	class Ant;

	class AntFactory
	{
		static bool IsIDPresent(const int ID);

	public:

		static int CreateAnt(const std::string& antType, const float x, const float y);
		static Ant* GetAntByID(const int ID);
		static void RemoveAntByID(const int ID);
	};

}

#endif
