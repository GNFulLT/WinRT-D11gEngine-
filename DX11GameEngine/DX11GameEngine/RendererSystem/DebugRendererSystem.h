#pragma once

#include "IRendererSystem.h"
#include "Common/Device/IDeviceResources.h"
#include <memory>

using namespace GNF::Common;

namespace GNF::Renderer
{
	class DebugRendererSystem : public IRendererSystem
	{
		public:
			DebugRendererSystem(Graphic::IRenderer3* renderer);
			virtual void Draw(Entity::IDrawableEntity*);
			virtual void Cls() override;
			//! Set Render Target
			virtual void StartDraw() override;
			virtual void InitResources() override
			{
				m_renderer->InitResources();

				D3D11_BLEND_DESC blendDesc;
				ZeroMemory(&blendDesc, sizeof(blendDesc));

				D3D11_RENDER_TARGET_BLEND_DESC rtbd;
				ZeroMemory(&rtbd, sizeof(rtbd));

				rtbd.BlendEnable = true;
				rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
				rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
				rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
				rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
				rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
				rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
				rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

				blendDesc.AlphaToCoverageEnable = false;
				blendDesc.RenderTarget[0] = rtbd;

				ComPtr<ID3D11BlendState> Transparency;

				pResources->GetD3DDevice()->CreateBlendState(&blendDesc, &Transparency);

				D3D11_RASTERIZER_DESC cmdesc;
				ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

				cmdesc.FillMode = D3D11_FILL_SOLID;
				cmdesc.CullMode = D3D11_CULL_NONE;
				cmdesc.MultisampleEnable = pResources->IsMultiSamplingEnabled();

				ComPtr<ID3D11RasterizerState> CCWcullMode;
				cmdesc.FrontCounterClockwise = false;
				auto hrr = pResources->GetD3DDevice()->CreateRasterizerState(&cmdesc, &CCWcullMode);

				pResources->GetD3DContext()->RSSetState(CCWcullMode.Get());

				float blendFactor[] = { 1.f, 1.f, 1.f, 1.f };

				pResources->GetD3DContext()->OMSetBlendState(Transparency.Get(), blendFactor, 0xffffffff);

			}
			virtual void UpdateThings() override
			{
				m_renderer->UpdateThings();
			}

			virtual Common::Device::IDeviceResources* GetResources() override
			{
				return pResources;
			}

			virtual void Render() override;
			virtual int SystemID() override
			{
				return 6;
			}

			//! Swap Chain Flip
			virtual void EndDraw() override;

			//! Dependents
		private:

			std::unique_ptr<Graphic::IRenderer3> m_renderer;
			ComPtr<ID3D11RasterizerState> m_rasterizerState;
			Device::IDeviceResources* pResources;
			
	};
}