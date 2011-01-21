#include "NurseBlackboard.h"


namespace AntZerg
{
	NurseBlackboard::NurseBlackboard(const float movement_speed, const int behavior, 
		const int curAction, const double delta_sum, const float target_x, const float target_y, const int target_ID)
		: Blackboard(behavior, curAction, delta_sum), movement_speed(movement_speed), target_x(target_x),
		target_y(target_y), target_ID(target_ID)
	{
	}

	float NurseBlackboard::GetTargetX() const
	{
		return target_x;
	}

	float NurseBlackboard::GetTargetY() const
	{
		return target_y;
	}

	int NurseBlackboard::GetTargetID() const
	{
		return target_ID;
	}

	float NurseBlackboard::GetMovementSpeed() const
	{
		return movement_speed;
	}

	void NurseBlackboard::SetTargetX(const float x)
	{
		target_x = x;
	}

	void NurseBlackboard::SetTargetY(const float y)
	{
		target_y = y;
	}

	void NurseBlackboard::SetTargetID(const int ID)
	{
		target_ID = ID;
	}

	luabind::scope NurseBlackboard::RegisterLua()
	{
		using namespace luabind;
		return class_<NurseBlackboard, Blackboard>("NurseBlackboard")
			.property("target_x", &NurseBlackboard::GetTargetX, &NurseBlackboard::SetTargetX)
			.property("target_y", &NurseBlackboard::GetTargetY, &NurseBlackboard::SetTargetY)
			.property("target_ID", &NurseBlackboard::GetTargetID, &NurseBlackboard::SetTargetID)
			.property("movement_speed", &NurseBlackboard::GetMovementSpeed);
	}
}