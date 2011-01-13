#include "AntWarehouse.h"


namespace AntZerg
{
	const int STARTING_FOOD = 50;
	const int WITHDRAW_AMT = 1;

	AntWarehouse::AntWarehouse(const float x, const float y)
		: storedFood(STARTING_FOOD), x(x), y(y)
	{
	}

	AntWarehouse::~AntWarehouse()
	{
	}

	void AntWarehouse::DepositFood(int deposit)
	{
		if(deposit)
		{
			storedFood += deposit;
		}
	}

	int AntWarehouse::GetStoredFood() const
	{
		return storedFood;
	}

	float AntWarehouse::GetX() const
	{
		return x;
	}

	float AntWarehouse::GetY() const
	{
		return y;
	}

	luabind::scope AntWarehouse::RegisterLua()
	{
		using namespace luabind;
		return class_<AntWarehouse>("AntWarehouse")
			.def("DepositFood", &AntWarehouse::DepositFood)
			.def("GetStoredFood", &AntWarehouse::GetStoredFood)
			.def("GetX", &AntWarehouse::GetX)
			.def("GetY", &AntWarehouse::GetY)
			.def("WithdrawFood", &AntWarehouse::WithdrawFood);
	}

	int AntWarehouse::WithdrawFood()
	{
		if(storedFood)
		{
			storedFood--;
			return WITHDRAW_AMT;
		}
		return 0;
	}
}