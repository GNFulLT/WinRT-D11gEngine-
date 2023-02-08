#include "pch.h"
#include "TextureBindable.h"
#include "Core/Game.h"
#include <boost/filesystem.hpp>
namespace GNF::Core::Bindable::Miscellaneous
{
	TextureBindable::TextureBindable(Texturing::ITextureLoader* textureLoader, const Image::IImage* img)
	{
		auto res = textureLoader->CreateTextureFromImage2D(img, m_texture.GetAddressOf());
		GThrowIfFailed(res);
	}

	void TextureBindable::Bind()
	{
		ID3D11ShaderResourceView* views[] = { m_texture.Get() };

		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->PSSetShaderResources(0, 1,
			views);
		
	}
}