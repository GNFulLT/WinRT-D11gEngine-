#include "pch.h"
#include "Scene.h"


namespace GNF::Core::Scene
{
	Scene::Scene(UINT width, UINT height, DXGI_FORMAT format, ID3D11Device3* device, ID3D11DeviceContext3* context)
		:m_width(width),m_height(height)
	{
		m_entityManager.reset(new EntityManager());
		m_sceneRenderer.reset(new Renderer::SceneRenderer(width, height, format, device, context));
	}

	void Scene::Init()
	{
		m_entityManager->Init();
		m_sceneRenderer->Init();
	}
	ID3D11ShaderResourceView* Scene::GetSceneFrame()
	{
		return m_sceneRenderer->GetFrame();
	}

	void Scene::AsRenderTarget()
	{
		m_sceneRenderer->SetAsRenderTarget();
		m_sceneRenderer->Clear();
	}

	void Scene::SceneSizeChanged(UINT width, UINT height)
	{
		m_sceneRenderer->ChangeResolution(width, height);
	}

	//!: Render Things
	void Scene::Render()
	{

	}

	//!: Update Entities
	void Scene::Update()
	{

	}
}