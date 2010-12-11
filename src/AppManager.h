#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <irrlicht.h>

namespace AntZerg
{

class AppManager
{
public:

	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver *driver;
	irr::scene::ISceneManager *smgr;
	irr::gui::IGUIEnvironment *guienv;

	AppManager();
	~AppManager();
};

}

#endif
