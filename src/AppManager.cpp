#include "AppManager.h"

using namespace irr;

namespace AntZerg
{

AppManager::AppManager()
{
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600),
			32, false, false, false, 0);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
}

AppManager::~AppManager()
{
	device->drop();
}

}
