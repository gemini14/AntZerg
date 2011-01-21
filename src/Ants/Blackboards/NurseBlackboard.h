#ifndef NURSEBLACKBOARD_H
#define NURSEBLACKBOARD_H


#include "Blackboard.h"


namespace AntZerg
{
	class NurseBlackboard : public Blackboard
	{
	public:

		float target_x;
		float target_y;
		int target_ID;
		float movement_speed;

		NurseBlackboard(const float movement_speed = 0.f, const int behavior = 0, const int curAction = 0,
			const double delta_sum = 0.0, const float target_x = 0.f, const float target_y = 0.f,
			const int target_ID = 0);

		float GetTargetX() const;
		float GetTargetY() const;
		int GetTargetID() const;
		float GetMovementSpeed() const;

		void SetTargetX(const float x);
		void SetTargetY(const float y);
		void SetTargetID(const int ID);
		
		static luabind::scope RegisterLua();
	};
}

#endif