#include "pch.h"

#include "IImage.h"

using namespace DirectX;

namespace GNF::Image
{
	const size_t IImage::GetWidth() const
	{
		return m_image.GetMetadata().width;
	}

	const size_t IImage::GetHeight() const
	{
		return m_image.GetMetadata().height;
	}

	const uint8_t* IImage::GetData() const
	{
		return m_rawImage->pixels;
	}

	const size_t IImage::GetRowPitch() const
	{
		return m_rawImage->rowPitch;
	}

	const size_t IImage::GetSlicePitch() const
	{
		return m_rawImage->slicePitch;
	}

	const DXGI_FORMAT IImage::GetFormat() const
	{
		return m_image.GetMetadata().format;
	}

	const bool IImage::IsCubemap() const noexcept
	{
		return m_image.GetMetadata().IsCubemap();
	}

	void IImage::InitPrivateData()
	{
		m_imageCount = m_image.GetImageCount();

		if (m_imageCount == 1)
		{
			m_rawImage = m_image.GetImage(0,0,0);
		}
		else
		{
			m_rawImage = m_image.GetImages();
		}
	}
}