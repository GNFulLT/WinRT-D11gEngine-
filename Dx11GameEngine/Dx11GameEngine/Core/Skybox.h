#pragma once

#include "Texture/TextureInfo.h"

namespace GNF::Core
{
	class Skybox
	{
	public:
		void Init(const wchar_t* imgPath);

		void Draw();
	private:
		Texture::TextureID m_textureId = 0;
	};
}