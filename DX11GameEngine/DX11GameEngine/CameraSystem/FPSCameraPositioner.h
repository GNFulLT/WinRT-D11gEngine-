#pragma once

#include "ICameraPositioner.h"
#include "InputSystem/IInputSystem.h"


namespace GNF::Camera 
{
	class FPSCameraPositioner final : public ICameraPositioner
	{
	public:
		FPSCameraPositioner() : m_pos(0,0,0.f),m_up(0,1,0) 
		{
			m_orientation = glm::lookAt(m_pos, glm::vec3(0, 0, -1), m_up);
		}
		//m_orientation(DirectX::SimpleMath::Quaternion::LookRotation(DirectX::SimpleMath::Vector3(0,0,10), m_up))

		FPSCameraPositioner(glm::vec3& pos, glm::vec3& target)
			: m_pos(pos),
			m_up(0,1,0),
			m_orientation(glm::lookAt(m_pos,target,m_up))
			// DirectX::SimpleMath::Matrix::CreateLookAt(pos, target, m_up)
		{}
		FPSCameraPositioner(glm::vec3& pos, glm::vec3& target, glm::vec3& up)
			: m_pos(pos), 
			  m_up(up),
			  m_orientation(glm::lookAt(m_pos, target, m_up))
			//DirectX::XMMatrixLookAtLH(pos,target,up)
		{}
		virtual glm::mat4 GetWorldViewProjMatrix(const glm::mat4& world) override;
		glm::mat4 GetViewMatrix() override;
		glm::vec3 GetPosition() override;
		void Update(const float frameTime) override;
		void InitResources() override;
		glm::mat4 GetViewProjMatrix() override;

		inline static glm::mat4 m_proj = glm::perspectiveLH(45.0f, 0.75f, 1.f, 1000.f);

	private:

		void UpdateUpVector();

		Input::MouseState pressedState;

		bool m_isFirstPress = true;

		float m_mouseSpeed = 0.01f;
		float m_acceleration = 150.0f;
		float m_damping = 0.2f;            // For slowing
		float m_maxSpeed = 10.0f;
		float m_fastCoef = 10.0f;
		bool m_isFast = false;

		inline static const glm::vec3  s_stoppingVector = glm::vec3(0, 0, 0);


	private:
		glm::quat m_orientation;
		glm::mat4 m_or;
		glm::vec3 m_pos;
		glm::vec3 m_up;
		glm::vec3 m_moveSpeed = glm::vec3(0, 0, 0);
		
		//DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.4f * 3.14f, 0.75f, 1.0f, 1000.0f);
		//		DirectX::SimpleMath::Matrix m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.4f * 3.14f, 0.75f, 1.0f, 1000.0f);
	};
}