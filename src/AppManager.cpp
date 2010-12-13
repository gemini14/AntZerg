#include "AppManager.h"

using namespace irr;

namespace AntZerg
{

	AppManager::AppManager(const int width, const int height)
	{
		video::E_DRIVER_TYPE driverType;
#ifdef _WIN32
		driverType = video::EDT_DIRECT3D9;
#else
		driverType = video::EDT_OPENGL;
#endif

		device = createDevice(driverType, core::dimension2d<u32>(width, height), 32, false, false, false, 0);
		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();
	}

	AppManager::~AppManager()
	{
		device->drop();
	}

}
