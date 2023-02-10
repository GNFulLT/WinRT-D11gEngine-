#include "pch.h"
#include "TextureManager.h"

#include "Game.h"
#include "Common/Utils/utils_common.h"
#include <boost/filesystem.hpp>
namespace GNF::Core
{
	void LogDebug(const char* msg)
	{
		Common::Logger::LogDebug(msg);
	}

	void LogCritical(const char* msg)
	{
		Common::Logger::LogCritical(msg);
	}

	void TextureManager::Init()
	{
		Common::Logger::LogDebug("Effect Factory is creating for texturing");
		m_effectFactory.reset(new DirectX::EffectFactory(Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice()));
		Common::Logger::LogDebug("Effect Factory is created");
		Common::Logger::LogDebug("DDSTextureLoader is creating for texturing");
		m_ddsTextureLoader.reset(new Texturing::DDSTextureLoader(Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice(),&LogDebug,&LogCritical));
		m_hdrTextureLoader.reset(new Texturing::HDRTextureLoader(Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice(), &LogDebug, &LogCritical));
		m_wicTextureLoader.reset(new Texturing::WICTextureLoader(Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice(), &LogDebug, &LogCritical));

		Common::Logger::LogDebug("DDSTextureLoader is created");
		InitDefaultState();
	}
	void TextureManager::InitDefaultState()
	{
		Common::Logger::LogDebug("Default Sampler State creating");
		Microsoft::WRL::ComPtr<ID3D11SamplerState> state;

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		GThrowIfFailed(Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice()->CreateSamplerState(&samplerDesc, state.GetAddressOf()));
		
		Common::Logger::LogDebug("Default Sampler State created");

		m_stateMap.emplace(Texture::DEFAULT, state);

	}

	bool TextureManager::IsTextureAvailable(Texture::TextureID id)
	{
		return m_textureMap.find(id) != m_textureMap.end();
	}

	bool TextureManager::ChangeImageToCubemap(Image::ImageID id)
	{
		Image::IImage* selectedImage = nullptr ;
		auto hr = S_OK;
		switch (m_imageMap[id]->GetImageType())
		{
		case Image::DDS:
			hr = m_ddsTextureLoader->ConvertImageToCubemap(m_imageMap[id].get(), &selectedImage);
			break;
		case Image::HDR:
			hr = m_hdrTextureLoader->ConvertImageToCubemap(m_imageMap[id].get(),&selectedImage);
			break;
		default:
			throw std::runtime_error("Not implemented image type for cubemapping");
			break;
		}

		if (FAILED(hr))
		{
			return false;
		}

		if (selectedImage->IsCubemap())
		{
			m_imageMap.erase(id);
			m_imageMap.emplace(id, selectedImage);
			return true;
		}
	
		return false;
	}

	void TextureManager::BindTexture(Texture::TextureID id,Texture::TextureState state)
	{
		if (id == 0)
		{
			if (m_bindedTexture == nullptr)
				return;
			ID3D11ShaderResourceView* const pSRV[1] = { NULL };
			ID3D11SamplerState* sampler[] = {NULL};
			Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->PSSetSamplers(0, 1, sampler);
			Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->PSSetShaderResources(0, 1, pSRV);
			m_bindedTexture = nullptr;
			m_textureState = Texture::NONE;
			return;
		}
		if (auto texture = m_textureMap.find(id); texture != m_textureMap.end())
		{
			switch (state)
			{
			case Texture::DEFAULT:
			{
				if (m_textureState == Texture::DEFAULT && (size_t)m_bindedTexture == (size_t)texture->second.get())
				{
					return;
				}
				m_textureState = Texture::DEFAULT;
				ID3D11SamplerState* sampler[] = { m_stateMap[Texture::DEFAULT].Get() };
				Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->PSSetSamplers(0, 1, sampler);
				texture->second->Bind();
				m_bindedTexture = texture->second.get();
				break;
			}
			default:
				Common::Logger::LogCritical("Unknown texture state or not impelmented texture state using default texture state");
				texture->second->Bind();
				break;
			}
			
		}
		else
		{
			Common::Logger::LogCritical(std::format("Wanted texture is not in map. Id : {}",id).c_str());
		}
	}

	Texture::TextureID TextureManager::CreateTexture(const wchar_t* filepath)
	{
		Image::IImage* pImage;

		if (FAILED(LoadeImage(filepath, &pImage)))
		{
			return 0;
		}
		
		auto texture = new Core::Bindable::Miscellaneous::TextureBindable(m_ddsTextureLoader.get(), pImage);
		Texture::TextureID textureId = Common::Utils::CreateUniqueID();

		m_textureMap.emplace(textureId, texture);

		delete pImage;

		return textureId;
	}

	Image::ImageID TextureManager::CreateImage(const wchar_t* filePath)
	{
		Image::IImage* img;
		auto hr = LoadeImage(filePath, &img);
		if (FAILED(hr))
			return 0;
		Image::ImageID imgId = Common::Utils::CreateUniqueID();
		
		m_imageMap.emplace(imgId, img);
		bool a = img->IsCubemap();
		return imgId;
	}

	Texture::TextureID TextureManager::CreateTextureFromImageAndDelete(Image::ImageID id)
	{
		auto texture = new Core::Bindable::Miscellaneous::TextureBindable(m_ddsTextureLoader.get(), m_imageMap[id].get());
		Texture::TextureID textureId = Common::Utils::CreateUniqueID();

		m_textureMap.emplace(textureId, texture);

		m_imageMap.erase(id);

		return textureId;
	}

	bool TextureManager::IsImageAvailable(Image::ImageID id)
	{
		return m_imageMap.find(id) != m_imageMap.end();
	}

	HRESULT TextureManager::LoadeImage(const wchar_t* filePath, Image::IImage** ppImage)
	{
		auto type = GetImageTypeFromPath(filePath);
		if (type == Image::UNKNOWN)
			return E_FAIL;


		HRESULT hr = S_OK;
		switch (type)
		{
		case Image::DDS:
			{
				hr = m_ddsTextureLoader->GLoadImage(filePath, ppImage);
				break;
			}
		case Image::HDR:
			{
				hr = m_hdrTextureLoader->GLoadImage(filePath, ppImage);
				break;
			}
		case Image::WIC:
			{
			hr = m_wicTextureLoader->GLoadImage(filePath, ppImage);
			break;
			}
		}
		
		return hr;
	}

	Image::ImageType TextureManager::GetImageTypeFromPath(const wchar_t* path)
	{
		boost::filesystem::path fullPath(path);

		if (!boost::filesystem::exists(fullPath))
		{
			return Image::UNKNOWN;
		}

		auto exts = fullPath.extension().wstring();
		
		//X: Be sure extension is lowercase

		return m_extensionMap[exts];
	}
	ID3D11ShaderResourceView1* TextureManager::CreateTextureAsShaderResource1()
	{
		return nullptr;
	}
}