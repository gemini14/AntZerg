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
		irr::video::ITexture *image1;
		irr::video::ITexture *image2;

	public:

		DisplayInfo(irr::video::IVideoDriver *driver, const float scale, 
			const std::string& texture1, const std::string& texture2);
		~DisplayInfo();
	};
}

#endif