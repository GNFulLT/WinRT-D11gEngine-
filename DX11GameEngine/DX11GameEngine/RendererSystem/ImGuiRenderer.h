#pragma once

#include "Graphic/IRenderer3.h"
#include "Common/Device/IDeviceResources.h"
#include <functional>
#include <vector>
namespace GNF::Renderer
{
	class ImGuiRenderer : public Graphic::IRenderer3
	{
		public:
			ImGuiRenderer(HWND hwnd,Common::Device::IDeviceResources* res) : m_hwnd(hwnd),p_res(res)
			{}
			virtual ~ImGuiRenderer();
			virtual void Render() override;

			virtual void InitResources() override;

			virtual void UpdateThings() override;
			

			inline virtual void PushImGuiRender(std::function<void()> imguiRenderFunc)
			{
				m_funcList.push_back(imguiRenderFunc);
			}

			virtual void Begin();

			virtual void Finish();

			virtual Common::Device::IDeviceResources* GetResources()
			{
				return p_res;
			}

		private:
			std::vector<std::function<void()>> m_funcList;
			HWND m_hwnd;
			Common::Device::IDeviceResources* p_res;
	};
}