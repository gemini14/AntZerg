#ifndef ANTWAREHOUSE_H
#define ANTWAREHOUSE_H


#include <boost/noncopyable.hpp>
#include <luabind/luabind.hpp>


namespace AntZerg
{
	class AntWarehouse : boost::noncopyable
	{
	private:

		int storedFood;
		float x;
		float y;

	public:

		AntWarehouse(const float x, const float y);
		~AntWarehouse();

		void DepositFood(int deposit);
		int GetStoredFood() const;
		float GetX() const;
		float GetY() const;
		static luabind::scope RegisterLua();
		int WithdrawFood(const int withdrawal);
	};
}


#endif