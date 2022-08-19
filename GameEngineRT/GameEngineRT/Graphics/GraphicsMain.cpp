#include "pch.h"
#include "GraphicsMain.h"
#include <sstream>
#include "Primitives/Triangle.h"
#include "Common/ShaderManager.h"


using namespace winrt::Windows::UI::Core;


namespace GNF::Graphics
{
	GraphicsMain::GraphicsMain()
	{
		m_deviceResources.reset(new Common::DeviceResources());

		m_graphRenderer.reset(new GraphicsRenderer(m_deviceResources.get()));

		m_gTimer.reset(new Common::GTimer());
	}
	float GraphicsMain::CalculateFrameStats()
	{
		//Static vars
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;
		//Start method
		static float lastFps = frameCnt;
		frameCnt++;
		if ((m_gTimer->GetTotalTimeSeconds() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			lastFps = fps;
			frameCnt = 0;
			timeElapsed += 1.0f;
			return fps;
		}
		return lastFps;
	}
	void GraphicsMain::SetWindow(winrt::Windows::UI::Core::CoreWindow const& window)
	{
		m_deviceResources->SetWindow(window);
		
	}

	void GraphicsMain::OnWindowSizeChanged(winrt::Windows::Foundation::Size newSize)
	{
		m_deviceResources->HandleWindowSizeChanged(newSize);
	}

	void GraphicsMain::Run()
	{
		m_gTimer->Reset();
		m_gTimer->Start();
		float deltaTime = 0;
		m_gTimer->Tick();
		
		Primitives::Triangle triangle(m_deviceResources->GetD3DDevice(), m_deviceResources->GetShaderManager()->GetVertexShader(Common::BasicVertexShader), m_deviceResources->GetShaderManager()->GetVertexShaderSize(Common::BasicVertexShader));

		while (true)
		{	
			std::wstringstream wsstream;
			wsstream << L"FPS : ";
			wsstream << std::to_wstring((int)CalculateFrameStats());
			UINT size = wsstream.seekp(0, wsstream.end).tellp();

			CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			//Start Draw Session
			m_graphRenderer->StartDraw();
			//CleanUp BG with Red Color
			m_graphRenderer->CleanUpBg(1, 0, 0);
			
			triangle.Draw(m_deviceResources->GetD3DContext());

			m_graphRenderer->DrawText(wsstream.str().c_str(), size);
			//Finish Draw Session
			m_graphRenderer->FinishDraw();
			
			deltaTime = m_gTimer->GetDeltaTimeSeconds();
			m_gTimer->Tick();

		}
	}
}