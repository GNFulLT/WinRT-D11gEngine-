#pragma once

#include <wrl/client.h>
#include <d3d11_4.h>
#include <unordered_map>
#include <memory>
#include "Texture/TextureInfo.h"
#include <DirectXTK/Effects.h>
#include "Core/Bindable/Miscellaneous/TextureBindable.h"
#include <DDSTextureLoader.h>
#include <HDRTextureLoader.h>
#include <WICTextureLoader.h>

namespace GNF::Texture
{
	enum TextureState
	{
		NONE = 0,DEFAULT
	};
}

namespace GNF::GUI
{
	class Icon;
}

namespace GNF::Image
{
	typedef unsigned long long ImageID;
}

namespace GNF::Core
{	
#define REX(str,imgType) {L#str,imgType}
	class TextureManager
	{
	public:
		//!: Depends on Graphic Engine
		void Init();
		Texture::TextureID CreateTexture(const wchar_t* filepath);

		Image::ImageID CreateImage(const wchar_t* filePath);
		
		Texture::TextureID CreateTextureFromImageAndDelete(Image::ImageID id);

		bool IsImageAvailable(Image::ImageID id);
		bool ChangeImageToCubemap(Image::ImageID id);
		void BindTexture(Texture::TextureID id,Texture::TextureState state = Texture::DEFAULT);
		bool IsTextureAvailable(Texture::TextureID id);
		Image::ImageType GetImageTypeFromPath(const wchar_t* path);
	private:
		HRESULT CreateTextureAsShaderResource1(const wchar_t* path,ID3D11ShaderResourceView1** ptr,Image::IImage** ppImage = nullptr);
		void InitDefaultState();
		HRESULT LoadeImage(const wchar_t* filePath,Image::IImage** pPImage);
	private:
		std::unordered_map<Texture::TextureID, std::unique_ptr<Core::Bindable::Miscellaneous::TextureBindable>> m_textureMap;

		std::unordered_map<Image::ImageID, std::unique_ptr<Image::IImage>> m_imageMap;

		std::unordered_map<Texture::TextureState, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_stateMap;
		std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
		std::unique_ptr<Texturing::DDSTextureLoader> m_ddsTextureLoader;
		std::unique_ptr<Texturing::HDRTextureLoader> m_hdrTextureLoader;
		std::unique_ptr<Texturing::WICTextureLoader> m_wicTextureLoader;
		Core::Bindable::Miscellaneous::TextureBindable* m_bindedTexture;
		Texture::TextureState m_textureState = Texture::NONE;
	private:
		
		inline static std::unordered_map<std::wstring, Image::ImageType> m_extensionMap =
		{
			REX(.dds,Image::DDS),
			REX(.hdr,Image::HDR),
			REX(.jpg,Image::WIC),
			REX(.jpeg,Image::WIC),
			REX(.png,Image::WIC),
			REX(.ico,Image::WIC),
			REX(.bmp,Image::WIC),
			REX(.tiff,Image::WIC)

		};

		friend class Core::Bindable::Miscellaneous::TextureBindable;
		friend class GNF::GUI::Icon;
	};
}