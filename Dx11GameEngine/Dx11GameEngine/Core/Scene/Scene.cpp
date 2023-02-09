#include "pch.h"
#include "Scene.h"


namespace GNF::Core::Scene
{
	Scene::Scene(UINT width, UINT height, DXGI_FORMAT format, ID3D11Device3* device, ID3D11DeviceContext3* context)
		:m_width(width),m_height(height)
	{
		m_entityManager.reset(new EntityManager());
		m_sceneRenderer.reset(new Renderer::SceneRenderer(width, height, format, device, context));
		m_textureManager.reset(new TextureManager());
	}

	void Scene::RenderSGui()
	{
		m_entityManager->RenderSGui();
	}

	void Scene::Init()
	{
		m_entityManager->Init();
		m_textureManager->Init();
		m_sceneRenderer->Init();
		
		auto id = m_textureManager->CreateTexture(L"Assets/1texture.dds");


		m_triangle = m_entityManager->CreateTriangle2D(1.f, { 0,1.f,0 }).lock();
		m_triangle->OnCreated();
		m_triangle->SetTexture(id);
		m_triangle->ReSetVerticesIndices();

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
		m_triangle->Draw();
	}

	//!: Update Entities
	void Scene::Update()
	{

	}
}