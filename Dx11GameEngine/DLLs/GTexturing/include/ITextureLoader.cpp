#include "pch.h"

#include "ITextureLoader.h"

namespace GNF::Texturing
{
	HRESULT ITextureLoader::CreateTextureFromImage2D(
		_In_ const Image::IImage* img,
		_Out_opt_ ID3D11ShaderResourceView1** srv,
		_In_opt_ D3D11_TEXTURE_LAYOUT layout,
		_In_opt_ D3D11_USAGE usage,
		_In_opt_ UINT cpuaccessFlag 
	)
	{
		auto device = GetDevice();
		
		if (!device)
		{
			OutputDebugString(L"Device was nullptr");
			return E_FAIL;
		}

		CD3D11_TEXTURE2D_DESC desc(img->GetFormat(), img->GetWidth(), img->GetHeight(),
			img->m_metaData.arraySize,img->m_metaData.mipLevels,8U,usage,cpuaccessFlag);

		CD3D11_TEXTURE2D_DESC1 desc1(desc, layout);

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = img->m_rawImage->pixels;
		initData.SysMemPitch = img->m_rawImage->rowPitch;
		initData.SysMemSlicePitch = img->m_rawImage->slicePitch;

		ID3D11Texture2D1* texture;
		auto hr = device->CreateTexture2D1(&desc1, &initData,&texture );
		
		if (FAILED(hr))
		{
			return hr;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC1 srvDesc1(texture, D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D);

		hr = device->CreateShaderResourceView1(texture, &srvDesc1, srv);

		return hr;
	}
	HRESULT ITextureLoader::ConvertImageToCubemap(_In_ Image::IImage* pImage, _Out_opt_ Image::IImage** outImg)
	{
		if (outImg == nullptr)
			return E_INVALIDARG;
		//!: Split image into 4 smaller image
		const auto newWidth = pImage->GetWidth() / 3;
		const auto newHeight = pImage->GetHeight() / 4;
		
		
		auto img = CreateInstanceFromResponsibleImage();
		DirectX::ScratchImage* cubeFace = &img->m_image;
		auto hr = cubeFace->InitializeCube(pImage->GetFormat(), newWidth, newHeight, 1, 1);

		if (FAILED(hr))
		{
			return hr;
		}
		
		auto src = pImage->m_image.GetImages()->pixels;
		auto dst = cubeFace->GetImages()->pixels;
		auto pixelSize = DirectX::BitsPerPixel(pImage->GetFormat());
		
		//!: Iterate faces
		for (int face = 0; face != 6; ++face)
		{
			//!: Iterate all height
			for (int j = 0; j != newHeight; ++j)
			{
				for (int i = 0; i != newWidth; ++i)
				{
					int x = 0;
					int y = 0;
					switch (face)
					{
						// TEXTURE_CUBE_MAP_POSITIVE_X

					case 0:
						x = i;
						y = newHeight + j;
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_X
					case 1:
						x = 2 * newWidth + i;
						y = newHeight + j;
						break;

						// TEXTURE_CUBE_MAP_POSITIVE_Y
					case 2:
						x = 2 * newWidth - (i + 1);
						y = newHeight - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_Y
					case 3:
						x = 2 * newWidth - (i + 1);
						y = 3 * newHeight - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_POSITIVE_Z
					case 4:
						x = 2 * newWidth - (i + 1);
						y = pImage->GetHeight() - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_Z
					case 5:
						x = newWidth + i;
						y = newHeight + j;
						break;
					}
					/*
					int selectedImageIndex = y / newHeight;
					auto selectedImage = cubeFace->GetImage(0, selectedImageIndex, 0);
					auto selectedBegin = selectedImage->pixels;*/
					memcpy(dst, src + (y * pImage->GetWidth() + x) * pixelSize/8, pixelSize/8);
					dst += pixelSize/8;

				}
			}
		}

		(*outImg) = img;
	
		img->InitPrivateData();
		img->m_metaData = img->m_image.GetMetadata();
	}
}