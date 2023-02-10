#include "pch.h"

#include "Game.h"
#include "../Common/Windowing/Win32/Win32_Window.h"
#include "Core/VertexBuffer/OnlyVertexBuffer.h"
#include "Common/Camera/CameraPositioner/FPSCameraPositioner.h"
#include <DirectXTK/GeometricPrimitive.h>
#include "Entitiy/Triangle2D.h"
#include "imgui/imgui_internal.h"

#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

namespace GNF::Core
{
	Game::Game(const char* gameName)
	{

		auto wnd = new Common::Windowing::Win32::Win32_Window(gameName, 1280, 960);
		engine.SetDPI(wnd->GetDPI());
		wnd->SetOnCreatingWindowCallback(std::bind(&Game::OnWindowCreated, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		wnd->SetOnResizeWindowCallback(std::bind(&Game::OnWindowResized, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		m_currentCamera = "FPS Camera";
		m_window.reset(wnd);
		m_menuBar.reset(new GUI::MenuBar());
		m_textureManager.reset(new TextureManager());
		m_skybox.reset(new Skybox());
		//m_scene.reset(new Scene::Scene());
		//m_entityNode.reset(new GUI::EntityNode());

	}
	void Game::OnWindowResized(HWND hwnd, UINT width, UINT height, bool isFullScreen, UINT dpi)
	{	
		//CalculateFrameScreenBounds();
		//m_newFrameSizeShouldBeCalculated = true;
		engine.Resize(width, height, isFullScreen, dpi);
		m_sizeChangedSignal(width, height, isFullScreen);
		m_swapState = SwapState::NeedResize;
		//m_layoutNeedValidate = true;
	}

	void Game::ValidateLayout(ImGuiID m_dockId)
	{
		ImGui::DockBuilderRemoveNode(m_dockId);

		ImGui::DockBuilderAddNode(m_dockId, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(m_dockId, ImGui::GetMainViewport()->Size);

		//ImGuiID center = m_dockId;

		auto dock_id_left = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Left,
			m_layout.leftAlignment, nullptr, &m_dockId);

		float rightAlign = (m_layout.rightAlignment) / (1.f - m_layout.leftAlignment);

		auto dock_id_right = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Right,
			rightAlign, nullptr, &m_dockId);

		ImGuiID dock_id_down;
		auto dock_id_up = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Up,
			m_layout.topAlignment, nullptr, &dock_id_down);

		/*auto dock_id_down = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Down,
			m_layout.bottomAlignment, nullptr, &m_dockId);
		*/

		

		ImGuiID dock_id_right_up;
		ImGuiID dock_id_right_down = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down,
			m_layout.rightDownAlignment, nullptr, &dock_id_right_up);

		ImGui::DockBuilderDockWindow("Scene", dock_id_left);
		ImGui::DockBuilderDockWindow("Output", dock_id_up);
		ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_right_up);
		ImGui::DockBuilderDockWindow("Global Settings", dock_id_right_down);
		ImGui::DockBuilderDockWindow("Styles", dock_id_right_up);

		ImGui::DockBuilderDockWindow("Properties", dock_id_right_down);
		ImGui::DockBuilderFinish(m_dockId);

		/*
		ImGuiID center = m_dockId;
		ImGuiID left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.25f, nullptr, &center);
		ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &center);

		ImGuiID rightDown;
		ImGuiID rightUp = ImGui::DockBuilderSplitNode(right, ImGuiDir_Up, 0.65f, nullptr, &rightDown);
		auto dock_id_top = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Up, 0.2f, nullptr, &m_dockId);
		auto dock_id_down = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Down, 0.25f, nullptr, &m_dockId);
		auto dock_id_left = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Left, 0.2f, nullptr, &m_dockId);
		auto dock_id_right = ImGui::DockBuilderSplitNode(m_dockId, ImGuiDir_Right, 0.15f, nullptr, &m_dockId);
		*/
		/*
		auto vp = ImGui::GetMainViewport();
		UINT width = (1.f - (m_layout.leftAlignment + m_layout.rightAlignment)) * m_window->GetWidth();
		UINT height = m_layout.topAlignment * m_window->GetHeight();
		*/
	}

	//void Game::CalculateFrameScreenBounds()
	//{

	//	m_frameScreenSize.x = (1.f - (m_layout.rightAlignment + m_layout.leftAlignment)) * m_window->GetWidth();
	//	m_frameScreenSize.y = m_layout.topAlignment * m_window->GetHeight();
	//}

	void Game::FrameSizeChanged()
	{
		//m_scene->SceneSizeChanged(m_frameSize.x, m_frameSize.y);
	}
	void Game::PreRenderSGui()
	{
		m_menuBar->RenderSGui();


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGuiID m_dockId = ImGui::GetID("##MainDocker");
		ImGui::PopStyleVar(2);

		/*
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize
			| ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::SetNextWindowDockID(m_dockId, ImGuiCond_::ImGuiCond_Always);

		*/
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize
			| ImGuiDockNodeFlags_PassthruCentralNode);

		if (m_layoutNeedValidate)
		{
			ValidateLayout(m_dockId);
			//CalculateFrameScreenBounds();
			m_layoutNeedValidate = false;

		}

		//m_entityManager->RenderSGui();

		ImGui::ShowDemoWindow();

		/*
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize
			| ImGuiDockNodeFlags_PassthruCentralNode);
		*/
		/*
		ImGui::Begin("Styles");
		ImGui::ShowStyleEditor();
		ImGui::End();
		*/

		//ImGui::ShowDemoWindow();
		//m_scene->RenderSGui();
		ImGui::Begin("Global Settings");
		ImGui::Text(std::format("FPS		 : {}", m_fps).c_str());
		ImGui::Text(std::format("Delta Time : {}", m_deltaTime).c_str());
		if (ImGui::TreeNode("Camera"))
		{
			if (ImGui::BeginCombo("Camera Types", m_currentCamera.c_str()))
			{
				if (m_currentCameraIndex != 0)
				{
					std::string str = "FPS Camera";
					bool selected = false;
					ImGui::Selectable(str.c_str(), &selected);
					if (selected)
					{
						m_currentCamera = str;
						m_currentCameraIndex = 0;
					}
				}

				if (m_currentCameraIndex != 1)
				{
					std::string str = "Move To Camera";
					bool selected = false;
					ImGui::Selectable(str.c_str(), &selected);
					if (selected)
					{
						m_currentCamera = str;
						m_currentCameraIndex = 1;
					}
				}
				ImGui::EndCombo();
			}
			m_camera->RenderGui();
			ImGui::TreePop();

		}
		//m_entityNode->RenderSGui();
		ImGui::End();

		ImGui::End();
	}
	void Game::RenderSGui()
	{
		/*
		if (m_newFrameSizeShouldBeCalculated)
		{
			m_newFrameSizeShouldBeCalculated = false;
			PreRender();
		}
		*/
		m_menuBar->RenderSGui();

	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGuiID m_dockId = ImGui::GetID("##MainDocker");
		ImGui::PopStyleVar(2);

		/*
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize 
			| ImGuiDockNodeFlags_PassthruCentralNode);
		
		ImGui::SetNextWindowDockID(m_dockId, ImGuiCond_::ImGuiCond_Always);
		
		*/
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize
			| ImGuiDockNodeFlags_PassthruCentralNode);

		if (m_layoutNeedValidate)
		{
			ValidateLayout(m_dockId);
			//CalculateFrameScreenBounds();
			m_layoutNeedValidate = false;
			
		}

		//m_entityManager->RenderSGui();
		
		ImGui::ShowDemoWindow();

		/*
		ImGui::DockSpace(m_dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_NoResize
			| ImGuiDockNodeFlags_PassthruCentralNode);
		*/
		
		ImGui::Begin("Styles");
		ImGui::ShowStyleEditor();
		ImGui::End();
		
		
		//ImGui::ShowDemoWindow();
		m_scene->RenderSGui();
		ImGui::Begin("Global Settings");
		ImGui::Text(std::format("FPS		 : {}",m_fps).c_str());
		ImGui::Text(std::format("Delta Time : {}", m_deltaTime).c_str());
		if (ImGui::TreeNode("Camera"))
		{
			if (ImGui::BeginCombo("Camera Types", m_currentCamera.c_str()))
			{
				if (m_currentCameraIndex != 0)
				{
					std::string str = "FPS Camera";
					bool selected = false;
					ImGui::Selectable(str.c_str(), &selected);
					if (selected)
					{
						m_currentCamera = str;
						m_currentCameraIndex = 0;
					}
				}

				if (m_currentCameraIndex != 1)
				{
					std::string str = "Move To Camera";
					bool selected = false;
					ImGui::Selectable(str.c_str(), &selected);
					if (selected)
					{
						m_currentCamera = str;
						m_currentCameraIndex = 1;
					}
				}
				ImGui::EndCombo();
			}
			m_camera->RenderGui();
			ImGui::TreePop();

		}
		//m_entityNode->RenderSGui();
		ImGui::End();
		
		ImGui::End();

	}

	void Game::OnWindowCreated(HWND hwnd, UINT width, UINT height, bool isFullScreen)
	{
		//CalculateFrameScreenBounds();
		engine.SetWindow(hwnd,width,height,isFullScreen, GetDpiForWindow(hwnd));

		m_vertexShader.reset(engine.CreateVertexShader<GNF::Core::VertexBuffer::OnlyVertexBuffer>(L"./SimpleVertexShader.cso"));
		m_pixelShader.reset(engine.CreatePixelShader(L"./SimplePixelShader.cso"));
		
		m_statistic.reset(new Common::Statistic::Statistic());
			
		m_fpsCameraPositioner.reset(new GNF::Common::Camera::CameraPositioner::FPSCameraPositioner({1.0f,1.0f,2.f},(float)(float)1920/(float)1080));
		m_camera.reset(new GNF::Common::Camera::Camera(m_fpsCameraPositioner.get()));
		m_imgui.reset(new Renderer::ImGuiRenderer(hwnd,engine.GetD3DDevice(),engine.GetD3DContext()));
		
		m_imgui->Init();

		m_menuBar->Init(engine.GetD3DContext());
		/*m_entityManager->Init();
		m_entityNode->Init();*/
		
		//m_imgui->PushRenderFunc(std::bind(&Game::RenderSGui,this));
		
	}

	Game::~Game()
	{
		int a = 5;
	}
	Common::Windowing::Keyboard::IKeyboard* Game::GetKeyboard()
	{
		return m_window->GetKeyboard();
	}

	Common::Windowing::Mouse::IMouse* Game::GetMouse()
	{
		return m_window->GetMouse();
	}

	void Game::Destroy()
	{

		engine.Destroy();
		g_instance.reset();
	}
	Game* Game::Build()
	{
		return Build("GEngine");
	}

	Game* Game::Build(const char* gameName)
	{
		g_instance.reset(new Game(gameName));
		return GetInstance();
	}

	void Game::Init()
	{
		//m_entityManager.reset(new EntityManager());
		engine.Init();
		m_textureManager->Init();
		m_window->Init();
		m_skybox->Init(L"Assets/bg.hdr");
	}
	std::weak_ptr<Renderer::ImGuiRenderer> Game::GetImGuiRenderer()
	{
		return m_imgui;
	}
	void Game::FixedRender()
	{
		engine.SetRenderTarget();
		engine.SetViewPort();

		m_imgui->Begin();
		PreRenderSGui();
		ImGui::Begin("Output", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

		m_frameSize = ImGui::GetContentRegionAvail();
		ImGui::End();

		m_imgui->Finish();
		
		FrameSizeChanged();

	}

	void Game::PreRender()
	{
		engine.SetRenderTarget();
		engine.SetViewPort();
		engine.ClearColor(0.f, 1.f, 0.f, 1.f);
		
		m_imgui->Begin();
		m_menuBar->Init_PreRender();
		PreRenderSGui();
		ImGui::Begin("Output", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
		
		m_frameSize = ImGui::GetContentRegionAvail();
		m_scene.reset(new Scene::Scene(m_frameSize.x, m_frameSize.y, engine.GetSwapChainFormat(), engine.GetD3DDevice(), engine.GetD3DContext()));
		m_scene->Init();
		ImGui::End();

		m_imgui->Finish();
		m_window->HandleEventsIfAny();
	}

	void Game::Render()
	{
		m_camera->Use(DirectX::SimpleMath::Matrix::Identity);
		
		engine.DrawTriangleStrip(3);
	}

	void Game::Run()
	{
		
		double tickCount = 0;
		double endTick = 0;
		//m_menuBar->Init();
		PreRender();
		//auto triWeak = m_entityManager->CreateTriangle2D(1.f,{0,1.f,0});
		//auto triWeak2 = m_entityManager->CreateTriangle2D(2.f, { 5.f,1.f,0 });

		//m_triangle = triWeak.lock();
		//m_triangle1 = triWeak2.lock();
		//
		//m_triangle1->OnCreated();
		//m_triangle->OnCreated();
		//m_triangle->ReSetVerticesIndices();
		//m_triangle1->ReSetVerticesIndices();
		//m_triangle->SetTexture(id);
		while (m_window->IsOpen())
		{
			endTick = tickCount;
			tickCount = m_statistic->GetDeltaTime();
			auto delta = (tickCount-endTick)/1000.f;
			m_deltaTime = delta;
			if (auto fps = m_statistic->GetFPS();fps != -1)
			{
				m_fps = fps;
			
			}

		
			//!: Engine
			//! 
			//! 
			//! 
			m_camera->Update(delta);


			/*
			engine.FrameBuffer_SetRenderTarget();
			engine.FrameBuffer_SetViewPort();
			engine.FrameBuffer_ClearColor();

			m_vertexShader->Bind();

			m_pixelShader->Bind();

			//!: Render Scene Method. Creates the Frame Buffer Texture
			Render();
			*/
			
			m_scene->AsRenderTarget();


			m_vertexShader->Bind();

			m_pixelShader->Bind();
			m_scene->Update();

			m_scene->Render();

			m_skybox->Draw();

			auto frame = m_scene->GetSceneFrame();
			
			engine.SetRenderTarget();
			engine.SetViewPort();
			engine.ClearColor(1.f, 0.f, 0.f, 1.f);
			
			
			m_imgui->Begin();
			RenderSGui();

			ImGui::Begin("Scene");
				
			ImGui::End();
			
			ImGui::Begin("Output",0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::Image(frame, m_frameSize);

			/*
			if (m_newFrameSizeCalculated)
			{
				m_frameSize = ImGui::GetContentRegionAvail();
				//m_frameSize.x = size1.x * (1.f - (m_layout.leftAlignment + m_layout.rightAlignment));
				FrameSizeChanged();
				m_newFrameSizeCalculated = false;
			}
			else
			{
				ImGui::Image(frame, m_frameSize);
			}
			*/
			ImGui::End();
			m_imgui->Finish();
			


			switch (m_swapState)
			{
			case Swap:
				engine.SwapBuffers();
				m_window->HandleEventsIfAny();
				break;
			case NeedResize:
				engine.SwapBuffers();
				m_window->HandleEventsIfAny();
				FixedRender();
				m_swapState = Swap;
				break;
			}

			//!: SwapBuff
			/*
			if (m_newFrameSizeShouldBeCalculated)
			{
				m_newFrameSizeCalculated = true;
				m_newFrameSizeShouldBeCalculated = false;
				m_waitForFrame = true;
			}
			else
			{
				if (m_waitForFrame)
				{
					m_waitForFrame = false;
				}
				else
				{
					engine.SwapBuffers();
				}
			}
			*/

		}
	}
}