#include "pch.h"

#include "DDSTextureLoader.h"
#include <boost/filesystem.hpp>
#include <string>
#include "Image/DDSImage.h"
namespace GNF::Texturing
{
	DDSTextureLoader::DDSTextureLoader(ID3D11Device3* device)
	{
		m_device = device;
	}
	DDSTextureLoader::DDSTextureLoader(ID3D11Device3* device,std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical)
	{
		m_device = device;
		m_logDebug = logDebug;
		m_logCritical = logCritical;
		m_debugMode = true;
	}

	HRESULT DDSTextureLoader::GLoadImage(
		_In_ const wchar_t* path,
		_Out_opt_ Image::IImage** img
	)
	{
		if (m_debugMode)
		{
			return GLoadImage_DBG(path, img);
		}
		else
		{
			return GLoadImage_NONDBG(path, img);
		}
	}

	ID3D11Device3* DDSTextureLoader::GetDevice()
	{
		return m_device;
	}

	Image::IImage* DDSTextureLoader::CreateInstanceFromResponsibleImage()
	{
		return new Image::DDSImage();
	}

	HRESULT DDSTextureLoader::GLoadImage_DBG(
		_In_ const wchar_t* path,
		_Out_opt_ Image::IImage** img
	)
	{
		if (img == nullptr)
		{
			OutputDebugString(L"Given Image pointer was nullptr");
			return E_INVALIDARG;
		}

		(*img) = nullptr;

		boost::filesystem::path fullPath(path);
		
		if (!boost::filesystem::exists(fullPath))
		{
			m_logCritical("File is not exist");
			return E_INVALIDARG;
		}

		if (!IsFileDDS(fullPath.filename().wstring().c_str()))
		{
			m_logCritical("File is not dds");
			return E_INVALIDARG;
		}
		
		Image::DDSImage* imag = new Image::DDSImage();
		HRESULT hr = DirectX::GetMetadataFromDDSFile(path, (DirectX::DDS_FLAGS)m_selectedFlag,imag->m_metaData);

		if (FAILED(hr))
		{
			m_logCritical(std::format("Couldn't loaded dds file returned with code : {}", hr).c_str());
			return hr;
		}

		hr = DirectX::LoadFromDDSFile(path, (DirectX::DDS_FLAGS)m_selectedFlag, &imag->m_metaData, imag->m_image);

		if (FAILED(hr))
		{
			m_logCritical(std::format("Couldn't loaded dds file returned with code : {}",hr).c_str());
			return hr;
		}
		
		(*img) = imag;
		imag->InitPrivateData();
		return S_OK;
	}

	void DDSTextureLoader::UseDefaultFormat()
	{
		
	}

	HRESULT DDSTextureLoader::GLoadImage_NONDBG(
		_In_ const wchar_t* path,
		_Out_opt_ Image::IImage** img
	)
	{
		if (img == nullptr)
		{
			OutputDebugString(L"Given Image pointer was nullptr");
			return E_INVALIDARG;
		}

		(*img) = nullptr;


		boost::filesystem::path fullPath(path);

		if (!boost::filesystem::exists(fullPath))
		{
			return E_INVALIDARG;
		}

		if (!IsFileDDS(fullPath.filename().wstring().c_str()))
		{
			return E_INVALIDARG;
		}
		
		Image::DDSImage* imag = new Image::DDSImage();
		HRESULT hr = DirectX::GetMetadataFromDDSFile(path, (DirectX::DDS_FLAGS)m_selectedFlag, imag->m_metaData);

		if (FAILED(hr))
		{
			return hr;
		}


		hr = DirectX::LoadFromDDSFile(path, (DirectX::DDS_FLAGS)m_selectedFlag, &imag->m_metaData, imag->m_image);

		if (FAILED(hr))
		{
			return hr;
		}

		(*img) = imag;

		return S_OK;
	}

	bool DDSTextureLoader::IsFileDDS(const wchar_t* fileName)
	{
		std::wstring fn = fileName;
		if (fn.substr(fn.find_last_of(L".")) == L".dds") {
			
			return true;
		}
		else {
			
			return false;
		}
	}
}