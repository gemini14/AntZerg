#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <boost/noncopyable.hpp>
#include <irrlicht.h>

namespace AntZerg
{

	class AppManager : boost::noncopyable
	{
	public:

		irr::IrrlichtDevice *device;
		irr::video::IVideoDriver *driver;
		irr::scene::ISceneManager *smgr;
		irr::gui::IGUIEnvironment *guienv;

		AppManager(const int width, const int height);
		~AppManager();
	};

}

#endif
