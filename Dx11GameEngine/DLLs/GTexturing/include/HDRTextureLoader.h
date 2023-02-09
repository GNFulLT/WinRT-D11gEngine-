#pragma once

#ifdef GTEXTURING_EXPORTS
#define GTEXTURING_API __declspec(dllexport)
#else
#define GTEXTURING_API __declspec(dllimport)
#endif

#include "ITextureLoader.h"
#include <functional>
#include "DirectXTex/DirectXTex.h"

namespace GNF::Texturing
{
	class GTEXTURING_API HDRTextureLoader final : public ITextureLoader
	{
	public:
		HDRTextureLoader(ID3D11Device3* device);
		HDRTextureLoader(ID3D11Device3* device,std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical);

		HRESULT GLoadImage(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		) override;

	private:
		bool IsFileHDR(const wchar_t* fileName);
	
		HRESULT GLoadImage_DBG(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		);

		HRESULT GLoadImage_NONDBG(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		);
		ID3D11Device3* GetDevice() override;

	private:
		ID3D11Device3* m_device;
	private:
		std::function<void(const char*)> m_logDebug;
		std::function<void(const char*)> m_logCritical;
		bool m_debugMode = false;
	};
}