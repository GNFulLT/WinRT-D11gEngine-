#include "pch.h"
#include "Icon.h"

namespace GNF::GUI
{
	Icon::Icon(const wchar_t* filePath, Core::TextureManager* manager, float width, float height) : m_size(width,height)
	{
		Image::IImage* ptr;
		auto hr = manager->CreateTextureAsShaderResource1(filePath, m_resourceView.GetAddressOf(),&ptr);
		if (SUCCEEDED(hr))
			m_isLoaded = true;
		m_fullSize.x = ptr->GetWidth();
		m_fullSize.y = ptr->GetHeight();
		delete ptr;
	}

	float Icon::GetFullWidth() const noexcept
	{
		return m_fullSize.x;
	}

	float Icon::GetFullHeight() const noexcept
	{
		return m_fullSize.y;
	}

	float Icon::GetWidth() const noexcept
	{
		return m_size.x;

	}

	float Icon::GetHeight() const noexcept
	{
		return m_size.y;
	}

	bool Icon::IsLoaded() const noexcept
	{
		return m_isLoaded;
	}
	void Icon::SetSize(float width, float height)
	{
		m_size.x = width;
		m_size.y = height;
	}
	void Icon::Draw()
	{
		ImGui::Image(m_resourceView.Get(), m_size);
	}
}