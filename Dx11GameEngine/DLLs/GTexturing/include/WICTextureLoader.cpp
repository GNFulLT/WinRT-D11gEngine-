#include "pch.h"
#include "WICTextureLoader.h"
#include <boost/filesystem.hpp>
#include <string>
#include "Image/WICImage.h"

namespace GNF::Texturing
{
	WICTextureLoader::WICTextureLoader(ID3D11Device3* device)
	{
		m_device = device;
	}
	WICTextureLoader::WICTextureLoader(ID3D11Device3* device, std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical)
	{
		m_device = device;
		m_logDebug = logDebug;
		m_logCritical = logCritical;
		m_debugMode = true;
	}
	HRESULT WICTextureLoader::GLoadImage(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
	{
		if (m_debugMode)
		{
			return GLoadImage_DBG(path, ppImg);
		}
		else
		{
			return GLoadImage_NONDBG(path, ppImg);
		}
	}
	bool WICTextureLoader::IsFileWIC(const wchar_t* fileName)
	{
		std::wstring fn = fileName;
		auto exs = fn.substr(fn.find_last_of(L"."));
		if (exs == L".jpg" || exs == L".png" || exs == L".jpeg" || exs == L".bmp" || exs == L".ico" || exs == L".tiff") {

			return true;
		}
		else {

			return false;
		}
	}
	HRESULT WICTextureLoader::GLoadImage_DBG(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
	{
		if (ppImg == nullptr)
		{
			OutputDebugString(L"Given Image pointer was nullptr");
			return E_INVALIDARG;
		}

		(*ppImg) = nullptr;

		boost::filesystem::path fullPath(path);

		if (!boost::filesystem::exists(fullPath))
		{
			m_logCritical("File is not exist");
			return E_INVALIDARG;
		}

		if (!IsFileWIC(fullPath.filename().wstring().c_str()))
		{
			m_logCritical("File is not WIC");
			return E_INVALIDARG;
		}

		Image::WICImage* imag = new Image::WICImage();
		auto hr = DirectX::GetMetadataFromWICFile(path, (DirectX::WIC_FLAGS)m_currentFlag,imag->m_metaData);

		if (FAILED(hr))
		{
			if (hr == E_NOINTERFACE)
			{
				m_logDebug(std::format("COM objs not found. Initialize COM", hr).c_str());
				hr = CoInitialize(nullptr);
				if (FAILED(hr))
				{
					m_logCritical("Couldn't loaded COM");
					return hr;
				}
				hr = DirectX::GetMetadataFromWICFile(path, (DirectX::WIC_FLAGS)m_currentFlag, imag->m_metaData);
				if (FAILED(hr))
				{
					m_logCritical("Couldn't loaded metadata");
					return hr;
				}
			}
			else
			{
				m_logCritical(std::format("Couldn't loaded metadata of WIC file returned with code : {}", hr).c_str());
				return hr;
			}
		}
		
		hr = DirectX::LoadFromWICFile(path, (DirectX::WIC_FLAGS)m_currentFlag,&imag->m_metaData, imag->m_image);

		if (FAILED(hr))
		{
			if (hr == E_NOINTERFACE)
			{
				m_logDebug(std::format("COM objs not found. Initialize COM", hr).c_str());
				hr = CoInitialize(nullptr);
				if (FAILED(hr))
				{
					m_logCritical("Couldn't loaded COM");
					return hr;
				}
			}
			else
			{
				m_logCritical(std::format("Couldn't loaded WIC file returned with code : {}", hr).c_str());
				return hr;
			}
			
		}

		(*ppImg) = imag;
		imag->InitPrivateData();
		return S_OK;

	}
	HRESULT WICTextureLoader::GLoadImage_NONDBG(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
	{
		if (ppImg == nullptr)
		{
			return E_INVALIDARG;
		}

		(*ppImg) = nullptr;

		boost::filesystem::path fullPath(path);

		if (!boost::filesystem::exists(fullPath))
		{
			return E_INVALIDARG;
		}

		if (!IsFileWIC(fullPath.filename().wstring().c_str()))
		{
			return E_INVALIDARG;
		}

		Image::WICImage* imag = new Image::WICImage();
		auto hr = DirectX::GetMetadataFromWICFile(path, (DirectX::WIC_FLAGS)m_currentFlag, imag->m_metaData);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = DirectX::LoadFromWICFile(path, (DirectX::WIC_FLAGS)m_currentFlag, &imag->m_metaData, imag->m_image);

		if (FAILED(hr))
		{
			return hr;
		}

		(*ppImg) = imag;
		imag->InitPrivateData();
		return S_OK;
	}
	ID3D11Device3* WICTextureLoader::GetDevice()
	{
		return m_device;
	}
	Image::IImage* WICTextureLoader::CreateInstanceFromResponsibleImage()
	{
		return new Image::WICImage();
	}
}