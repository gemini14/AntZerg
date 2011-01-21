#include "LarvaBlackboard.h"


namespace AntZerg
{
	LarvaBlackboard::LarvaBlackboard(const int behavior, const int curAction, const double delta_sum, bool dead)
		: Blackboard(behavior, curAction, delta_sum), dead(dead)
	{
	}

	bool LarvaBlackboard::GetDead() const
	{
		return dead;
	}

	void LarvaBlackboard::SetDead(bool d)
	{
		dead = d;
	}

	luabind::scope LarvaBlackboard::RegisterLua()
	{
		using namespace luabind;
		return class_<LarvaBlackboard, Blackboard>("LarvaBlackboard")
			.property("dead", &LarvaBlackboard::GetDead, &LarvaBlackboard::SetDead);
	}
}