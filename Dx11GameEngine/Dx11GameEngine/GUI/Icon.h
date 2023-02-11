#pragma once

#include <d3d11_4.h>
#include "Core/TextureManager.h"

namespace GNF::GUI
{
	class Icon
	{
	public:
		Icon(const wchar_t* filePath,Core::TextureManager* manager,float width = 0,float height = 0);
		Icon(const Icon&) = delete;
		Icon& operator=(const Icon&) = delete;
		void Draw();
		bool IsLoaded() const noexcept;
		void SetSize(float width, float height);
		float GetHeight() const noexcept;
		float GetWidth() const noexcept;
		float GetFullWidth() const noexcept;
		float GetFullHeight() const noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> m_resourceView;
		bool m_isLoaded = false;
		ImVec2 m_size = {0,0};
		ImVec2 m_fullSize = { 0,0 };
	};
}