#include "DisplayInfo.h"


namespace AntZerg
{
	DisplayInfo::DisplayInfo(irr::video::IVideoDriver *driver, const float scale, 
		const std::string& texture1, const std::string& texture2)
		: displayScale(scale)
	{
		image1 = driver->getTexture(texture1.c_str());
		image2 = driver->getTexture(texture2.c_str());
	}

	DisplayInfo::~DisplayInfo()
	{
		image1->drop();
		image2->drop();
	}
}