#include "GameMain.h"
#include "InputSystem/InputSystem.h"
#include <typeinfo>
#include <vcruntime_typeinfo.h>
#include "Window/MainWindow.h"
#include "Common/Timer/ITimerSystem.h"
#include "Common/Timer/GTimer.h"

#include "RendererSystem/DebugRendererSystem.h"
#include "Graphic/CameraTestRenderer.h"
#include "EntitySystem/Cube.h"
#include <string>

#include <imgui/imgui.h>

namespace GNF::Game
{
	GameMain::GameMain(const Window::WindowDesc& desc)
	{
		pMainWindow.reset(Window::MainWindow::CreateMainWindowInstance(desc));

		

		WithDefaultSystems();	
	}
	float GameMain::GetFrameTime()
	{
		return 1 / fps;
	}

	void GameMain::CalculateFPS()
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		auto m_timer = GetSystem<Timer::ITimerSystem>();

		// Compute averages over one second period.
		if ((m_timer->GetTotalTimeSeconds() - timeElapsed) >= 1.0f)
		{
			fps = (float)frameCnt;
			float mspf = 1000.0f / fps;

			std::wstring fpsStr = std::to_wstring(fps);
			std::wstring mspfStr = std::to_wstring(mspf);

			std::wstring windowText =
				L"fps: " + fpsStr +
				L"   mspf: " + mspfStr;

			SetWindowText(pMainWindow->GetHWnd(), windowText.c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	GameMain* GameMain::WithDefaultSystems()
	{

		const type_info& type = typeid(Input::IInputSystem);
		size_t hashCode = type.hash_code();

		const type_info& timerType = typeid(Timer::ITimerSystem);
		size_t timerHashCode = timerType.hash_code();
		

		const type_info& rendererType = typeid(Renderer::IRendererSystem);
		size_t renderHashCode = rendererType.hash_code();

		m_systemStorage[hashCode] = std::move(std::unique_ptr<Input::InputSystem>(new Input::InputSystem()));
		m_systemStorage[timerHashCode] = std::move(std::unique_ptr<Common::Timer::GTimer>(new Common::Timer::GTimer()));
		m_systemStorage[renderHashCode] = std::move(std::unique_ptr<Renderer::DebugRendererSystem>(new Renderer::DebugRendererSystem(
		new Graphic::CameraTestRenderer(((Window::MainWindow*)pMainWindow.get())->GetDeviceResources())
		
		)));

		CreateEntity(new Entity::Cube());
		CreateEntity(new Entity::Cube(2.f,{3.f,0,0}));

		cam.reset(new Camera::CameraSystem(fpsCam));
		cameraConstBuffer.reset(new Common::Bindable::ConstBufferBindable(GetMainWindow()->GetDeviceResources(), &perObj, sizeof(Camera::cbPerObject)));
		
		m_imgui.reset(new Renderer::ImGuiRenderer(pMainWindow->GetHWnd(),GetMainWindow()->GetDeviceResources()));

		return this;
	}

	Window::IWindow32* GameMain::GetNativeWindow()
	{
		return pMainWindow.get();
	}
	GameMain* GameMain::BuildGame()
	{
		Window::WindowDesc desc;
		desc.height = 480;
		desc.width = 640;
		desc.windowTitle = L"GNF WINDOW";
		desc.windowClassName = L"GNFWINDOW";

		return BuildGame(desc);
	}

	GameMain* GameMain::BuildGame(const Window::WindowDesc& desc)
	{
		g_gameMain.reset(new GameMain(desc));

		return g_gameMain.get();
	}
	void Render()
	{
		ImGui::Begin("Merhaba");

		ImGui::End();
	}
	int GameMain::Run()
	{
		auto m_timer = GetSystem<Timer::ITimerSystem>();

		m_timer->Reset();
		m_timer->Start();
		

		m_imgui->PushImGuiRender(Render);

		for (auto& entity : m_entities)
		{
			entity->InitResources();
		}
		auto renderer = GetSystem<Renderer::IRendererSystem>();
		renderer->InitResources();
		
		auto resources = GetMainWindow()->GetDeviceResources();
		CalculateFPS();

		
		glm::mat4 fworld = glm::mat4(1.0f);

		auto vm = cam->GetViewProjMatrix();

		//perObj.WVP = (cam->GetWorldViewProjMatrix(world)).Transpose();
		
		//perObj.WVP = (cam->GetWorldViewProjMatrix(world)).Transpose();
		
		perObj.WVP = glm::transpose(cam->GetWorldViewProjMatrix(world));
		cameraConstBuffer->Update(&perObj);

		cameraConstBuffer->Bind();

		m_imgui->InitResources();
		/*
		//! For Calculating Avarage FPS
		while (fps == 1)
		{
			CalculateFPS();

			std::optional<UINT> res = ((Window::MainWindow*)pMainWindow.get())->ProcessWindowMessagesIfExist();
			if (res.has_value())
			{
				switch (res.value())
				{
				case WM_QUIT:
					return 0;
					break;
				}
			}


			//! For Camera
			fpsCam.Update(m_timer->GetDeltaTimeSeconds());

			auto vm = cam->GetViewProjMatrix();

			//perObj.WVP = (cam->GetWorldViewProjMatrix(world)).Transpose();
			perObj.WVP = glm::transpose(cam->GetWorldViewProjMatrix(world));

			cameraConstBuffer->Update(&perObj);

			cameraConstBuffer->Bind();

			renderer->StartDraw();

			renderer->Cls();

			m_imgui->Begin();
			m_imgui->Render();
			m_imgui->Finish();

			for (auto& drawable : m_drawables)
			{
				drawable->Draw();
			}

			renderer->EndDraw();

			m_timer->Tick();
		}
		*/
		//! MAIN LOOP
		while (1)
		{
			CalculateFPS();

			std::optional<UINT> res = ((Window::MainWindow*)pMainWindow.get())->ProcessWindowMessagesIfExist();
			if (res.has_value())
			{
				switch (res.value())
				{
				case WM_QUIT:
					return 0;
					break;
				}
			}

			//! For Camera
			fpsCam.Update(m_timer->GetDeltaTimeSeconds());

			auto vm = cam->GetViewProjMatrix();
			
			//perObj.WVP = (cam->GetWorldViewProjMatrix(world)).Transpose();
			perObj.WVP = glm::transpose(cam->GetWorldViewProjMatrix(world));

			cameraConstBuffer->Update(&perObj);

			cameraConstBuffer->Bind();


			m_imgui->Begin();
			m_imgui->Render();
			m_imgui->Finish();


			for (auto& entity : m_entities)
			{
				entity->Update(1 / fps);
		
			}
			renderer->StartDraw();

			renderer->Cls();
						


			for (auto& drawable : m_drawables)
			{
				renderer->Draw(drawable);
			}

			renderer->EndDraw();
			
			m_timer->Tick();
			
		}
	}
}