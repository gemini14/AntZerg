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
	}

	float DisplayInfo::GetDisplayScale() const
	{
		return displayScale;
	}

	irr::video::ITexture * const DisplayInfo::GetTexture1() const
	{
		return image1;
	}

	irr::video::ITexture * const DisplayInfo::GetTexture2() const
	{
		return image2;
	}
}