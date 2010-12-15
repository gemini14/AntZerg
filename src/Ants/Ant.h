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

		bool movementEnabled;

		// this could be changed to another vector implementation later if needed
		irr::core::vector2df position;	
		
		float displayScalingFactor;
		
		std::string configFile;
		std::string actionScript;

		int food;
		
		int ID;

	public:

		Ant(const int ID, bool canMove, const std::string& configFile, 
			const std::string& actionScriptFile, const float x, const float y);
		virtual ~Ant();

		bool CanMove() const;
		void DecreaseFoodStock();
		virtual void Eat() = 0;
		float GetDispScaling() const;
		int GetFood() const;
		int GetID() const;
		irr::core::vector2df GetPosition() const;
		float GetX() const;
		float GetY() const;
		void IncreaseFoodStock();
		void SetScalingFactor(const float scale);
				

		virtual void Run() = 0;
	};
}

#endif
