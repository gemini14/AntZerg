#include "WorkerBlackboard.h"


namespace AntZerg
{
	WorkerBlackboard::WorkerBlackboard(const float movement_speed, const int behavior, const int curAction,
		const double delta_sum, const float target_x, const float target_y, const int target_ID, const int plot)
		: NurseBlackboard(movement_speed, behavior, curAction, delta_sum, target_x, target_y, target_ID), plot(plot)
	{
	}

	int WorkerBlackboard::GetPlot() const
	{
		return plot;
	}

	void WorkerBlackboard::SetPlot(const int plot)
	{
		this->plot = plot;
	}

	luabind::scope WorkerBlackboard::RegisterLua()
	{
		using namespace luabind;
		return class_<WorkerBlackboard, NurseBlackboard>("WorkerBlackboard")
			.property("plot", &WorkerBlackboard::GetPlot, &WorkerBlackboard::SetPlot);
	}
}