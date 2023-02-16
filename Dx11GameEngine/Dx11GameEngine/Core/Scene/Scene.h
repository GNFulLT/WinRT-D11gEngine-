#pragma once

#include "Renderer/SceneRenderer.h"
#include "EntityManager.h"
#include <memory>
#include "Core/IEngineManager_1.h"

namespace GNF::Core::Scene
{
	class Scene : public Core::IEngineManager_1
	{
	public:
		Scene(UINT width,UINT height,DXGI_FORMAT format,ID3D11Device3* device,ID3D11DeviceContext3* context);
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
		void SceneSizeChanged(UINT width, UINT height);

		void RenderSGui();

		void Subflow_Init() override;

		//!: Init Resources
		void Init();

		//!: Update Entities
		void Update();
		//!: Render Things
		void Render(ID3D11DeviceContext3* ctx);
		void AsRenderTarget(ID3D11DeviceContext3* ctx);
		ID3D11ShaderResourceView* GetSceneFrame();
		void PreRender();
	private:
		UINT m_width;
		UINT m_height;
	private:
		std::unique_ptr<Renderer::SceneRenderer> m_sceneRenderer;
		std::shared_ptr<Entity::IEntity> m_triangle;
		std::shared_ptr<Entity::IEntity> m_triangle2;
		std::unique_ptr<EntityManager> m_entityManager;
	};
}