#pragma once

#include <d3d11_4.h>
#include "Core/TextureManager.h"

namespace GNF::GUI
{
	class Icon
	{
	public:
		Icon(const wchar_t* filePath,Core::TextureManager* manager);
		void Draw();
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> m_resourceView;
	};
}