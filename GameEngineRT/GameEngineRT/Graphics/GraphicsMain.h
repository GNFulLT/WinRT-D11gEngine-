#pragma once
#include "GraphicsRenderer.h"
#include "Common/DeviceResources.h"
#include "Common/GTimer.h"

namespace GNF::Graphics
{

	class GraphicsMain
	{
		public:
			GraphicsMain();
			GraphicsMain(const GraphicsMain&) = delete;
			GraphicsMain& operator =(const GraphicsMain&) = delete;
			void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
			void OnWindowSizeChanged(winrt::Windows::Foundation::Size newSize);
			void Run();
		private:
			float CalculateFrameStats();
		private:
			std::unique_ptr<Common::DeviceResources> m_deviceResources;
			std::unique_ptr<GraphicsRenderer> m_graphRenderer;
			std::unique_ptr<Common::GTimer> m_gTimer;

	};

}
