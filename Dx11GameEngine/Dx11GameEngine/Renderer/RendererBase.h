#pragma once
#include <d3d11_4.h>

namespace GNF::Renderer
{
	class RendererBase
	{
	public:
		RendererBase(ID3D11Device3* device, ID3D11DeviceContext3* deviceContext) : p_device(device),p_deviceContext(deviceContext)
		{

		}
		virtual ~RendererBase() = default;
	protected:
		ID3D11Device3* p_device;
		ID3D11DeviceContext3* p_deviceContext;
	};
}