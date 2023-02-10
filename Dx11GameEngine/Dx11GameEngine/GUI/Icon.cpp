#include "pch.h"
#include "Icon.h"

namespace GNF::GUI
{
	Icon::Icon(const wchar_t* filePath, Core::TextureManager* manager, float width, float height) : m_size(width,height)
	{
		auto hr = manager->CreateTextureAsShaderResource1(filePath, m_resourceView.GetAddressOf());
		if (SUCCEEDED(hr))
			m_isLoaded = true;
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