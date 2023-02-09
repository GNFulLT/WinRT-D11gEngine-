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
	enum DDS_READ_FLAG
	{
		DEFAULT = 0,FORCE_RGB = 8UL
	};

	class GTEXTURING_API DDSTextureLoader final : public ITextureLoader
	{
	public:
		DDSTextureLoader(ID3D11Device3* device);
		DDSTextureLoader(ID3D11Device3* device,std::function<void(const char*)> logDebug, std::function<void(const char*)> logCritical);

		void UseDefaultFormat();
				
		HRESULT GLoadImage(
			_In_ const wchar_t* path,
			_Out_opt_ Image::IImage** ppImg
		) override;

	private:
		bool IsFileDDS(const wchar_t* fileName);

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
		DDS_READ_FLAG m_selectedFlag = DEFAULT;
	private:
		std::function<void(const char*)> m_logDebug;
		std::function<void(const char*)> m_logCritical;
		bool m_debugMode = false;
	};
}