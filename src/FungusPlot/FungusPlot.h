#ifndef FUNGUSPLOT_H
#define FUNGUSPLOT_H


#include <luabind/luabind.hpp>


namespace AntZerg
{
	class FungusPlot
	{
	private:

		int food;
		float x;
		float y;

	public:

		FungusPlot(const float x, const float y);
		~FungusPlot();

		int GetFood() const;
		float GetX() const;
		float GetY() const;
		static luabind::scope RegisterLua();
		int WithdrawFood(const int amount);
	};
}

#endif