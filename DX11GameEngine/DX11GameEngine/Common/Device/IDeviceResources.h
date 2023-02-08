#pragma once
#include <d3d11.h>
#include "Common/TextureManagement/ITextureManager.h"
namespace GNF::Common::Device
{
	class IDeviceResources
	{
		friend class DebugDeviceResources;

		public:
			virtual ~IDeviceResources() {}

			virtual void Init() = 0;

			virtual bool IsMultiSamplingEnabled() = 0;

			virtual float AspectRatio() const noexcept = 0;
			//X Put This to renderer 
			virtual void ClearBackgroundColor() = 0;

			virtual void HandleWindowSizeChanged() = 0;

			virtual ID3D11Device* GetD3DDevice() = 0;

			virtual ID3D11DeviceContext* GetD3DContext() = 0;

			virtual HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC*,UINT numElements,ID3D11InputLayout**) = 0;
			
			virtual void SwapChain() = 0;

			virtual void Cls() = 0;

			virtual void SetTargetViews() = 0;

			virtual Texture::ITextureManager* GetTextureManager() = 0;
		private:
			virtual void CreateDependentResources() = 0;

			virtual void CreateInDependentResources() = 0;

			
	};
}