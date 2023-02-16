#include "pch.h"
#include "Scene.h"
#include "../Game.h"

namespace GNF::Core::Scene
{
	Scene::Scene(UINT width, UINT height, DXGI_FORMAT format, ID3D11Device3* device, ID3D11DeviceContext3* context)
		:m_width(width),m_height(height),Core::IEngineManager_1("Scene")
	{
		m_entityManager.reset(new EntityManager());
		m_sceneRenderer.reset(new Renderer::SceneRenderer(width, height, format, device, context));
	}

	void Scene::Subflow_Init()
	{
		auto entityTask = AddAsSubflowTask(m_entityManager.get());
		auto rendererTask = AddAsSubflowTask(m_sceneRenderer.get());
		auto initTask = InitAsTask();

		//!: Should Init after entity manager loaded
		initTask.succeed(entityTask,rendererTask);

		//m_sceneRenderer->Init();
	}
	void Scene::PreRender()
	{
		m_entityManager->PreRender();
	}

	void Scene::RenderSGui()
	{
		m_entityManager->RenderSGui();
	}

	void Scene::Init()
	{	
		Common::Logger::LogDebug("Scene Init");
		auto id = Core::Game::GetInstance()->GetCurrentTextureManager()->CreateTexture(L"Assets/texture.jpg");

		m_triangle = m_entityManager->CreateTriangle2D(1.f, { 0,1.f,0 }).lock();
		m_triangle->OnCreated();
		m_triangle->SetTexture(id);
		m_triangle->ReSetVerticesIndices();

	}
	ID3D11ShaderResourceView* Scene::GetSceneFrame()
	{
		return m_sceneRenderer->GetFrame();
	}

	void Scene::AsRenderTarget(ID3D11DeviceContext3* ctx)
	{
		m_sceneRenderer->SetAsRenderTarget(ctx);
		m_sceneRenderer->Clear(ctx);
	}

	void Scene::SceneSizeChanged(UINT width, UINT height)
	{
		m_sceneRenderer->ChangeResolution(width, height);
	}

	//!: Render Things
	void Scene::Render(ID3D11DeviceContext3* ctx)
	{
		m_triangle->Draw(ctx);
	}

	//!: Update Entities
	void Scene::Update()
	{

	}
}