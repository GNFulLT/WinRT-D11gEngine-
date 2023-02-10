#pragma once

#include "IImage.h"

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif

namespace GNF::Texturing
{
	class WICTextureLoader;
}

namespace GNF::Image
{
	class GTEXTURING_API WICImage final : public IImage
	{
	public:
		WICImage() : IImage()
		{};
		WICImage(const WICImage&) = delete;
		inline ImageType GetImageType() const override
		{
			return imageType;
		}
	private:
		friend class Texturing::WICTextureLoader;
	private:
		constexpr inline static ImageType imageType = ImageType::WIC;
	};
}