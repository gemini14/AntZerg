#ifndef WORKERBLACKBOARD_H
#define WORKERBLACKBOARD_H


#include "NurseBlackboard.h"


namespace AntZerg
{
	class WorkerBlackboard : public NurseBlackboard
	{
	public:

		int plot;

		WorkerBlackboard(const float movement_speed = 0.f, const int behavior = 0, const int curAction = 0, 
			const double delta_sum = 0.0, const float target_x = 0, const float target_y = 0, 
			const int target_ID = 0, const int plot = -1);

		int GetPlot() const;
		void SetPlot(const int plot);

		static luabind::scope RegisterLua();
	};
}

#endif