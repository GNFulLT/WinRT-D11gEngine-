#include "pch.h"

#include "Skybox.h"
#include "Game.h"

namespace GNF::Core
{
	void Skybox::Init(const wchar_t* imgPath)
	{
		auto id = Core::Game::GetInstance()->GetCurrentTextureManager()->CreateImage(imgPath);
		
		if (id == 0)
			throw std::runtime_error("Skybox couldn't loaded");

		bool succeed = Core::Game::GetInstance()->GetCurrentTextureManager()->ChangeImageToCubemap(id);
		
		if(!succeed)
			throw std::runtime_error("Skybox couldn't loaded");
		
		m_textureId = Core::Game::GetInstance()->GetCurrentTextureManager()->CreateTextureFromImageAndDelete(id);

		if(m_textureId == 0)
			throw std::runtime_error("Skybox couldn't loaded");

	}
	void Skybox::Draw()
	{
	}
}