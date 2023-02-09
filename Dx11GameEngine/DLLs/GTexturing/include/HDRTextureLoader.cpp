#include "pch.h"
#include "HDRTextureLoader.h"
#include <boost/filesystem.hpp>
#include <string>
#include "Image/HDRImage.h"

namespace GNF::Texturing
{
	HDRTextureLoader::HDRTextureLoader(ID3D11Device3* device)
	{
		m_device = device;
	}
	HDRTextureLoader::HDRTextureLoader(ID3D11Device3* device, std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical)
	{
		m_device = device;
		m_logDebug = logDebug;
		m_logCritical = logCritical;
		m_debugMode = true;
	}
	HRESULT HDRTextureLoader::GLoadImage(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
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
	bool HDRTextureLoader::IsFileHDR(const wchar_t* fileName)
	{
		std::wstring fn = fileName;
		if (fn.substr(fn.find_last_of(L".")) == L".hdr") {

			return true;
		}
		else {

			return false;
		}
	}
	HRESULT HDRTextureLoader::GLoadImage_DBG(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
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

		if (!IsFileHDR(fullPath.filename().wstring().c_str()))
		{
			m_logCritical("File is not dds");
			return E_INVALIDARG;
		}

		Image::HDRImage* imag = new Image::HDRImage();
		auto hr = DirectX::GetMetadataFromHDRFile(path, imag->m_metaData);

		if (FAILED(hr))
		{
			m_logCritical(std::format("Couldn't loaded metadata of hdr file returned with code : {}", hr).c_str());
			return hr;
		}

		hr = DirectX::LoadFromHDRFile(path, &imag->m_metaData, imag->m_image);

		if (FAILED(hr))
		{
			m_logCritical(std::format("Couldn't loaded hdr file returned with code : {}", hr).c_str());
			return hr;
		}

		(*ppImg) = imag;
		imag->InitPrivateData();
		return S_OK;
	
	}
	HRESULT HDRTextureLoader::GLoadImage_NONDBG(_In_ const wchar_t* path, _Out_opt_ Image::IImage** ppImg)
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

		if (!IsFileHDR(fullPath.filename().wstring().c_str()))
		{
			return E_INVALIDARG;
		}

		Image::HDRImage* imag = new Image::HDRImage();
		auto hr = DirectX::GetMetadataFromHDRFile(path,imag->m_metaData);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = DirectX::LoadFromHDRFile(path, &imag->m_metaData, imag->m_image);
		
		if (FAILED(hr))
		{
			return hr;
		}

		(*ppImg) = imag;
		imag->InitPrivateData();
		return S_OK;
	}
	ID3D11Device3* HDRTextureLoader::GetDevice()
	{
		return m_device;
	}
	Image::IImage* HDRTextureLoader::CreateInstanceFromResponsibleImage()
	{
		return new Image::HDRImage();
	}
}