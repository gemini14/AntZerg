#include "DisplayInfo.h"


namespace AntZerg
{
	DisplayInfo::DisplayInfo(irr::video::IVideoDriver *driver, const float scale, 
		const std::string& texture)
		: displayScale(scale)
	{
		image = driver->getTexture(texture.c_str());
	}

	DisplayInfo::~DisplayInfo()
	{
	}

	float DisplayInfo::GetDisplayScale() const
	{
		return displayScale;
	}

	irr::video::ITexture * const DisplayInfo::GetTexture() const
	{
		return image;
	}
}