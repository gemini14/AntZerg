#ifndef BLACKBOARD_H
#define BLACKBOARD_H


#include <luabind/luabind.hpp>


namespace AntZerg
{
	class Blackboard
	{
	public:
		int behavior;
		int curAction;
		double delta_sum;

		Blackboard(const int behavior = 0, const int curAction = 0, const double delta_sum = 0.0);

		int GetBehavior() const;
		int GetCurAction() const;
		double GetDeltaSum() const;

		void SetBehavior(const int b);
		void SetCurAction(const int c);
		void SetDeltaSum(const double dt);

		static luabind::scope RegisterLua();
	};
}

#endif