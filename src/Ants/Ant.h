#ifndef ANT_H
#define ANT_H


#include <string>

#include <irrlicht.h>
#include <boost/noncopyable.hpp>


namespace AntZerg
{
	class Ant : boost::noncopyable
	{
	private:

		// this could be changed to another vector implementation later if needed
		irr::core::vector2df position;	
		
		float displayScalingFactor;
		
		std::string configFile;
		std::string actionScript;

		int ID;

	public:

		Ant(const std::string& configFile, const std::string& actionScriptFile, const float x, const float y);
		virtual ~Ant();

		float GetDispScaling() const;
		int GetID() const;
		irr::core::vector2df GetPosition() const;
		float GetX() const;
		float GetY() const;
		void SetScalingFactor(const float scale);

		virtual void Run() = 0;
	};
}

#endif
