#pragma once 

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif

#include <d3d11_4.h>
#include <DirectXTex/DirectXTex.h>
#include <cstdint>

namespace GNF::Texturing
{
	class ITextureLoader;
}

namespace GNF::Image
{
	enum ImageType
	{
		UNKNOWN = 0,DDS,HDR
	};

	class GTEXTURING_API IImage
	{
	public:
		virtual ~IImage() = default;

		virtual const size_t GetWidth() const;

		virtual const size_t GetHeight() const;

		virtual const uint8_t* GetData() const;

		virtual const size_t GetRowPitch() const;
		
		virtual const size_t GetSlicePitch() const;

		virtual const DXGI_FORMAT GetFormat() const;

		virtual ImageType GetImageType() const = 0;

		virtual const bool IsCubemap() const noexcept;

	protected:
		DirectX::ScratchImage m_image;
		DirectX::TexMetadata m_metaData;
		size_t m_imageCount = 0;
		const DirectX::Image* m_rawImage = nullptr;
		virtual void InitPrivateData();
	private:
		friend class Texturing::ITextureLoader;

	};
}