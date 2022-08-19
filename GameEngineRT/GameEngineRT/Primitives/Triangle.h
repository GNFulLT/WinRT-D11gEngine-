#pragma once

#include "Common/DeviceResources.h"
#include "Common/Bindable/IBindable.h"

namespace GNF::Primitives
{
	class Triangle
	{
		public:
			Triangle(ID3D11Device* resources,const void* byteShader, UINT const& shaderSize);
			void Draw(ID3D11DeviceContext*);
			ID3D11InputLayout* GetInputLayout();
		private:
			winrt::com_ptr<ID3D11Buffer> m_vertexBuffer;
			winrt::com_ptr<ID3D11InputLayout> m_inputLayout;

	};
}