#ifndef LARVABLACKBOARD_H
#define LARVABLACKBOARD_H


#include "Blackboard.h"


namespace AntZerg
{
	class LarvaBlackboard : public Blackboard
	{
	public:

		bool dead;

		LarvaBlackboard(const int behavior = 0, const int curAction = 0, const double delta_sum = 0.0, 
			bool dead = false);

		bool GetDead() const;
		void SetDead(bool d);

		static luabind::scope RegisterLua();
	};
}

#endif