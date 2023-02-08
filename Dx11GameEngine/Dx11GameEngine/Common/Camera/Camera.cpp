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