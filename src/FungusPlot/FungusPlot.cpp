#include "FungusPlot.h"


namespace AntZerg
{
	FungusPlot::FungusPlot(const float x, const float y)
		: food(500), x(x), y(y)
	{
	}

	FungusPlot::~FungusPlot()
	{
	}

	int FungusPlot::GetFood() const
	{
		return food;
	}

	float FungusPlot::GetX() const
	{
		return x;
	}

	float FungusPlot::GetY() const
	{
		return y;
	}

	luabind::scope FungusPlot::RegisterLua()
	{
		using namespace luabind;
		return class_<FungusPlot>("FungusPlot")
			.def("GetFood", &FungusPlot::GetFood)
			.def("GetX", &FungusPlot::GetX)
			.def("GetY", &FungusPlot::GetY)
			.def("WithdrawFood", &FungusPlot::WithdrawFood);
	}

	int FungusPlot::WithdrawFood(const int amount)
	{
		if(food >= amount)
		{
			food -= amount;
			return amount;
		}

		// not enough to fill amount, so take all remaining food (set to 0) and return any food removed
		int temp = food;
		food = 0;
		return temp;
	}
}