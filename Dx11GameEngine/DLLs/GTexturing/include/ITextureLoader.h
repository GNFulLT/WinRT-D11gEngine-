#pragma once

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif



#include <d3d11_4.h>
#include "Image/IImage.h"
namespace GNF::Texturing
{
	enum TextureLoadStrategy
	{
		TEXTURE_2D = 0
	};

	class GTEXTURING_API ITextureLoader
	{
	public:
		virtual ~ITextureLoader() = default;

		virtual HRESULT GLoadImage(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		) = 0;

		virtual ID3D11Device3* GetDevice() = 0;

		//!: If usage is Immutable please release the Image
		virtual HRESULT CreateTextureFromImage2D(
			_In_ const Image::IImage* ppImg,
			_Out_opt_ ID3D11ShaderResourceView1** srv,
			_In_opt_ D3D11_TEXTURE_LAYOUT layout = D3D11_TEXTURE_LAYOUT_UNDEFINED,
			_In_opt_ D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
			_In_opt_ UINT cpuaccessFlag = 0
		);

	private:
	};
}