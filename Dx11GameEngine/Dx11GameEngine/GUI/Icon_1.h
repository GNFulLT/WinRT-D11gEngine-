#pragma once

#include <memory>
#include "Icon.h"
#include "Common/IResource.h"

namespace GNF::GUI
{
	class Icon_1 : public Common::IResource
	{
	public:
		Icon_1(const wchar_t* filePath, Core::TextureManager* manager, float width = 0, float height = 0);
		Icon_1(const Icon_1&) = delete;
		Icon_1& operator=(const Icon_1&) = delete;
	
	//! IResource Async and Sync Initialization
	public:
		void Init() override;

	//!: Wrapper for icon
	public:
		inline void Draw()
		{
			m_icon->Draw();
		}
		
		inline bool IsLoaded() const noexcept
		{
			return m_icon->IsLoaded();
		} 

		inline void SetSize(float width, float height)
		{
			m_icon->SetSize(width, height);
		}

		inline float GetHeight() const noexcept
		{
			return m_icon->GetHeight();
		}

		inline float GetWidth() const noexcept
		{
			return m_icon->GetWidth();
		}

		inline float GetFullWidth() const noexcept
		{
			return m_icon->GetFullWidth();
		}

		inline float GetFullHeight() const noexcept
		{
			return m_icon->GetFullHeight();
		}

	private:
		float m_initialWidth;
		float m_initialHeight;
		Core::TextureManager* m_manager;
		std::wstring m_filePath;
		std::unique_ptr<Icon> m_icon;
	};
}