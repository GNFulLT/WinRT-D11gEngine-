#include "DebugRendererSystem.h"
#include "Game/GameMain.h"

namespace GNF::Renderer
{
	
	DebugRendererSystem::DebugRendererSystem(Graphic::IRenderer3* renderer)
	{
		m_renderer.reset(renderer);
		pResources = m_renderer->GetResources();
	}
	void DebugRendererSystem::Cls()
	{
		pResources->Cls();
	}
	void DebugRendererSystem::Draw(Entity::IDrawableEntity* drawable)
	{
		
		drawable->Draw();
	}

	//! Set Render Target
	void DebugRendererSystem::StartDraw()
	{
		pResources->SetTargetViews();
			
	}

	void DebugRendererSystem::Render()
	{
	
		


		m_renderer->Render();
	}

	//! Swap Chain Flip
	void DebugRendererSystem::EndDraw()
	{
		pResources->SwapChain();
	}
}