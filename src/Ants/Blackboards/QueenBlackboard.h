#ifndef QUEENBLACKBOARD_H
#define QUEENBLACKBOARD_H


#include "Blackboard.h"


namespace AntZerg
{
	class QueenBlackboard : public Blackboard
	{
	public:

		double larva_sum;

		QueenBlackboard(const int behavior = 0, const int curAction = 0, const double delta_sum = 0.0,
			const double larva_sum = 0.0);

		double GetLarvaSum() const;
		void SetLarvaSum(const double ldelta);

		static luabind::scope RegisterLua();
	};
}

#endif