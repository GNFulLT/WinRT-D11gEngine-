#pragma once

#include "IVertexBuffer.h"
#include "Core/GraphicEngine/GraphicEngine.h"

namespace GNF::Core::VertexBuffer
{
	struct OnlyVertexBuffer : public IVertexBuffer
	{
		float x;
		float y;
		float z;

		float tx;
		float ty;
		OnlyVertexBuffer(float x,float y,float z):x(x),y(y),z(z)
		{
			tx = 0.f;
			ty = 0.f;
		}
		
		OnlyVertexBuffer(float x, float y, float z, float tx, float ty) :x(x), y(y), z(z),tx(tx),ty(ty)
		{
			
		}

		inline static Microsoft::WRL::ComPtr<ID3D11InputLayout> g_inputLayout;
		inline static bool isInputLayoutCreated = false;
		
		static void Init(const char* byteCode,size_t size)
		{
			D3D11_INPUT_ELEMENT_DESC desc[] = {
					"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0,
				    "UV",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0
			};
			GraphicEngine::GraphicEngine::GetInstance()->GetD3DDevice()->CreateInputLayout(desc, ARRAYSIZE(desc), byteCode, size, g_inputLayout.GetAddressOf());
			isInputLayoutCreated = true;
		}
		
		static ID3D11InputLayout* GetInputLayout()
		{
			return g_inputLayout.Get();
		}
		static size_t GetInputLayoutIndex() 
		{
			return (size_t) g_inputLayout.Get();
		}
	};
}