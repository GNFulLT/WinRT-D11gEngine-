#include "pch.h"
#include "Camera.h"

#include "Core/GraphicEngine/GraphicEngine.h"
#include "Core/Game.h"
namespace GNF::Common::Camera
{
	using namespace CameraPositioner;
	using namespace Core::GraphicEngine;
	Camera::Camera(ICameraPositioner* camPositioner)
	{
		p_positioner = camPositioner;
	
		//data.totalTime = 0;
		m_constBuffer.reset(Core::GraphicEngine::GraphicEngine::GetInstance()->CreateDefaultConstantBuff(nullptr, sizeof(data)));

	}

	void Camera::SetCameraPositioner(ICameraPositioner* positioner)
	{
		p_positioner = positioner;
	
	}
	void Camera::RenderGui()
	{
		p_positioner->RenderGui();
	}

	ICameraPositioner* Camera::GetSelectedCameraPositioner()
	{
		return p_positioner;
	}

	void Camera::Use(const DirectX::SimpleMath::Matrix& modelMatrix, ID3D11DeviceContext3* ctx)
	{
		ID3D11Query* pQuery;
		D3D11_QUERY_DESC queryDesc = {};
		queryDesc.Query = D3D11_QUERY_EVENT;
		auto hr = Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice()->CreateQuery(&queryDesc, &pQuery);
		ctx->End(pQuery);
		pQuery->Release();
		const auto& vp = p_positioner->GetViewProjection();
		data.mvp = (modelMatrix * vp).Transpose();
		

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = ctx->Map(m_constBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, &data, sizeof(PerFrameData));
		ctx->Unmap(m_constBuffer->GetBuffer(), 0);

		//m_constBuffer->Update(&data);
		m_constBuffer->Bind(ctx);
	}

	void Camera::Use(const DirectX::SimpleMath::Matrix& modelMatrix)
	{
		const auto& vp = p_positioner->GetViewProjection();
		data.mvp = (modelMatrix * vp).Transpose();
		
		m_constBuffer->Update(&data);
		m_constBuffer->Bind();
	}

	void Camera::Update(const float deltaTime)
	{
		p_positioner->Update(deltaTime);
	}

}