#pragma once

#include "IImage.h"

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif

namespace GNF::Texturing
{
	class HDRTextureLoader;
}

namespace GNF::Image
{ 
	class GTEXTURING_API HDRImage final : public IImage
	{
	public:
		HDRImage() : IImage()
		{};
		HDRImage(const HDRImage&) = delete;
		inline ImageType GetImageType() const override
		{
			return imageType;
		}
	private:
		friend class Texturing::HDRTextureLoader;
	private:
		constexpr inline static ImageType imageType = ImageType::HDR;
	};
}