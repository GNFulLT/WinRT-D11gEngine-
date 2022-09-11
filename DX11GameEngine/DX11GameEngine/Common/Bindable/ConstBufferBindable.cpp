#pragma once

#include "ConstBufferBindable.h"
namespace GNF::Common::Bindable
{
	ConstBufferBindable::ConstBufferBindable(Device::IDeviceResources* resources, const void* data, UINT byteWidth) : pParentDevice(resources)
	{
		D3D11_BUFFER_DESC cbbd;
		ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
		cbbd.Usage = D3D11_USAGE_DEFAULT;
		cbbd.ByteWidth = byteWidth;
		cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbbd.CPUAccessFlags = 0;
		cbbd.MiscFlags = 0;

		resources->GetD3DDevice()->CreateBuffer(&cbbd, NULL, cbPerObjectBuffer.GetAddressOf());
		resources->GetD3DContext()->UpdateSubresource(cbPerObjectBuffer.Get(), 0, NULL, data, 0, 0);
	}
	void ConstBufferBindable::Update(const void* data)
	{
		pParentDevice->GetD3DContext()->UpdateSubresource(cbPerObjectBuffer.Get(), 0, NULL, data, 0, 0);

	}

	void ConstBufferBindable::Bind()
	{
		pParentDevice->GetD3DContext()->VSSetConstantBuffers(0, 1, cbPerObjectBuffer.GetAddressOf());

	}
}
