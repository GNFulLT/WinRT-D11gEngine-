#pragma once

#include "IImage.h"

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif

namespace GNF::Texturing
{
	class DDSTextureLoader;
}

namespace GNF::Image
{
	class GTEXTURING_API DDSImage final : public IImage
	{
	public:
		DDSImage() : IImage()
		{};
		DDSImage(const DDSImage&) = delete;
		inline ImageType GetImageType() const override
		{
			return imageType;
		}
	private:
		friend class Texturing::DDSTextureLoader;
	private:
		constexpr inline static ImageType imageType = ImageType::DDS;
	};
}