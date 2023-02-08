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
}