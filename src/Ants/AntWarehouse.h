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

		inline void DepositFood(int deposit);
		inline int GetStoredFood() const;
		inline float GetX() const;
		inline float GetY() const;
		static luabind::scope RegisterLua();
		int WithdrawFood(const int withdrawal);
	};
}


#endif