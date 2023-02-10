#pragma once

#include "Texture/TextureInfo.h"
#include <memory>
#include "Core/Bindable/Shader/VertexShaderBindable.h"
#include "Core/Bindable/Shader/PixelShaderBindable.h"
#include "Core/Bindable/Buffer/VertexBufferBindable.h"
#include "Core/Bindable/Buffer/IndexBufferBindable.h"
#include <wrl/client.h>
namespace GNF::Core
{
	class Skybox
	{
	public:
		void Init(const wchar_t* imgPath);

		void Draw();
	private:
		Texture::TextureID m_textureId = 0;
		DirectX::SimpleMath::Matrix m_scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(20,10,20));
		DirectX::SimpleMath::Matrix m_rotateMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(DirectX::SimpleMath::Quaternion(0,0,0,1.f));
		DirectX::SimpleMath::Matrix m_tranlateMatrix = DirectX::SimpleMath::Matrix::Identity;
		std::unique_ptr<Bindable::Shader::PixelShaderBindable> m_pixelShader;
		std::unique_ptr<Bindable::Shader::VertexShaderBindable> m_vertexShader;
		std::unique_ptr<Bindable::Buffer::VertexBufferBindable> m_vertexBuffer;
		std::unique_ptr<Bindable::Buffer::IndexBufferBindable> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	};
}