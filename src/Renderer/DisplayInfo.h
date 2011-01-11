#ifndef DISPLAYINFO_H
#define DISPLAYINFO_H


#include <string>

#include <irrlicht.h>


namespace AntZerg
{
	class DisplayInfo
	{
	private:

		float displayScale;
		irr::video::ITexture *image;
		
	public:

		DisplayInfo(irr::video::IVideoDriver *driver, const float scale, 
			const std::string& texture1);
		~DisplayInfo();

		float GetDisplayScale() const;
		irr::video::ITexture * const GetTexture() const;
	};
}

#endif