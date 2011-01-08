#ifndef RENDERER_H
#define RENDERER_H


#include <memory>

#include <boost/noncopyable.hpp>

#include "../AppManager.h"


namespace AntZerg
{
	class Renderer : boost::noncopyable
	{
	private:

		std::shared_ptr<AppManager> app;

	public:

		Renderer(std::shared_ptr<AppManager> app);
		~Renderer();

		void DrawAll();
	};
}

#endif