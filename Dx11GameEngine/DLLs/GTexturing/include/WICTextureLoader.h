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
	enum WIC_READ_FLAG
	{
		WIC_READ_FLAG_DEFAULT = 0
	};
	class GTEXTURING_API WICTextureLoader final : public ITextureLoader
	{
	public:
		WICTextureLoader(ID3D11Device3 * device);
		WICTextureLoader(ID3D11Device3* device,std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical);

		HRESULT GLoadImage(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		) override;


		static bool IsFileWIC(const wchar_t* fileName);
	private:
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
		virtual Image::IImage* CreateInstanceFromResponsibleImage() override;
		ID3D11Device3* m_device;
		WIC_READ_FLAG m_currentFlag = WIC_READ_FLAG::WIC_READ_FLAG_DEFAULT;
	private:
		std::function<void(const char*)> m_logDebug;
		std::function<void(const char*)> m_logCritical;
		bool m_debugMode = false;
	};
}