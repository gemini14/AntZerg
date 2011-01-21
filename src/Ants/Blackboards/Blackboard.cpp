#include "Blackboard.h"


namespace AntZerg
{
	Blackboard::Blackboard(const int behavior, const int curAction, const double delta_sum)
		: behavior(behavior), curAction(curAction), delta_sum(delta_sum)
	{
	}

	int Blackboard::GetBehavior() const
	{
		return behavior;
	}

	int Blackboard::GetCurAction() const
	{
		return curAction;
	}

	double Blackboard::GetDeltaSum() const
	{
		return delta_sum;
	}

	void Blackboard::SetBehavior(const int b)
	{
		behavior = b;
	}

	void Blackboard::SetCurAction(const int c)
	{
		curAction = c;
	}

	void Blackboard::SetDeltaSum(const double dt)
	{
		if(dt >= 0)
		{
			delta_sum += dt;
		}
		else
		{
			delta_sum = 0;
		}
	}

	luabind::scope Blackboard::RegisterLua()
	{
		using namespace luabind;
		return class_<Blackboard>("Blackboard")
			.property("behavior", &Blackboard::GetBehavior, &Blackboard::SetBehavior)
			.property("curAction", &Blackboard::GetCurAction, &Blackboard::SetCurAction)
			.property("delta_sum", &Blackboard::GetDeltaSum, &Blackboard::SetDeltaSum);
	}
}