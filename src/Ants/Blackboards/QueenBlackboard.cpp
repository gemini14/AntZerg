#include "QueenBlackboard.h"


namespace AntZerg
{
	QueenBlackboard::QueenBlackboard(const int behavior, const int curAction, const double delta_sum,
		const double larva_sum)
		: Blackboard(behavior, curAction, delta_sum), larva_sum(larva_sum)
	{
	}

	double QueenBlackboard::GetLarvaSum() const
	{
		return larva_sum;
	}

	void QueenBlackboard::SetLarvaSum(const double ldelta)
	{
		if(ldelta >= 0)
		{
			larva_sum += ldelta;
		}
		else
		{
			larva_sum = 0;
		}
	}

	luabind::scope QueenBlackboard::RegisterLua()
	{
		using namespace luabind;
		return class_<QueenBlackboard, Blackboard>("QueenBlackboard")
			.property("larva_sum", &QueenBlackboard::GetLarvaSum, &QueenBlackboard::SetLarvaSum);
	}
}