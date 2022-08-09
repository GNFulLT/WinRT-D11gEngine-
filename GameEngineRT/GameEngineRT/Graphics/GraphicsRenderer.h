#pragma once
#include "Common/DeviceResources.h"


namespace GNF::Graphics
{

	class GraphicsRenderer
	{

		public:
			GraphicsRenderer(Common::DeviceResources* resources);
			void CleanUpBg(float r,float g,float b);
			void Render();
			void StartDraw();
			void FinishDraw();
			void DrawText(const WCHAR* text,UINT textSize);
		private:
			Common::DeviceResources* s_deviceResources;
			winrt::com_ptr<ID2D1SolidColorBrush> m_blackBrush;

	};

}

