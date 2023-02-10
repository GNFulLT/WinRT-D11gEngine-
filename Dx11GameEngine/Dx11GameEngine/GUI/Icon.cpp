#include "pch.h"
#include "Icon.h"

namespace GNF::GUI
{
	Icon::Icon(const wchar_t* filePath, Core::TextureManager* manager)
	{
		manager->CreateTextureAsShaderResource1(filePath)
	}
	void Icon::Draw()
	{
	}
}