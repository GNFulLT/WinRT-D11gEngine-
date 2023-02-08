#define NOMINMAX


#include "FPSCameraPositioner.h"

#include "Game/GameMain.h"
#include "InputSystem/VirtualKeyCodes.h"
#include "Game/GameMain.h"
#include <iostream>
namespace GNF::Camera
{
	void FPSCameraPositioner::InitResources()
	{

	}

	void FPSCameraPositioner::UpdateUpVector()
	{
		//auto view = GetViewMatrix();
		//auto forward = view.Forward();
		//m_or = DirectX::SimpleMath::Matrix::CreateLookAt(m_pos,m_pos + forward, m_up);
	
		const glm::mat4 view = GetViewMatrix();
		const glm::vec3 dir = -glm::vec3(view[0][2], view[1][2], view[2][2]);
		m_orientation = glm::lookAt(m_pos, m_pos + dir, m_up);
	}

	glm::mat4 FPSCameraPositioner::GetViewProjMatrix()
	{
		return m_proj * GetViewMatrix();
	}


	glm::mat4 FPSCameraPositioner::GetViewMatrix()
	{
		/*auto t = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
		DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_orientation);
		return  (m_or * t);*/
		const glm::mat4 t = glm::translate(glm::mat4(1.0f), -m_pos);
		const glm::mat4 r = glm::mat4_cast(m_orientation);
		return r * t;
	
	}
	glm::vec3 FPSCameraPositioner::GetPosition()
	{
		return m_pos;
	}
	glm::mat4 FPSCameraPositioner::GetWorldViewProjMatrix(const glm::mat4& world)
	{
		return m_proj * GetViewMatrix() * world;
	}

	void FPSCameraPositioner::Update(const float frameTime)
	{
		auto input = Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>();
		glm::vec3 accel(0.0f);
		
		const Input::MouseState* cMouseState = nullptr;
		const Input::MouseState* prevMouseState = nullptr;
		input->GetPrevCurrMouseState(&prevMouseState, &cMouseState);
		
		if (input->IsMouseKeyPressed(Input::MOUSE_KEY_CODE::LEFT_BUTTON))
		{
			glm::vec2 delta = { float(pressedState.posx - cMouseState->posx ),float(pressedState.posy - cMouseState->posy) };
			std::cout << delta.x << " " << delta.y << std::endl;
			const glm::quat deltaQuat = glm::quat(glm::vec3(m_mouseSpeed * delta.y, m_mouseSpeed * delta.x, 0.0f));
			std::cout << "Quat : X : " << deltaQuat.x << " Y :" << deltaQuat.y << " Z : " << deltaQuat.z << " W : " << deltaQuat.w << std::endl;
			m_orientation = deltaQuat * m_orientation;
			m_orientation = glm::normalize(m_orientation);
			

			//UpdateUpVector();

			//m_or = m_or * DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotateQuat);

			int a = 5;
		}

		//DirectX::SimpleMath::Matrix r = m_or;

		//auto forward = r.Forward();
		//auto right = r.Right();
		
		//auto forward = m_or.Forward();
		//auto right = m_or.Right();

		const glm::mat4 v = glm::mat4_cast(m_orientation);

		const glm::vec3 forward = glm::vec3(v[0][2], v[1][2], v[2][2]);
		const glm::vec3 right = glm::vec3(v[0][0], v[1][0], v[2][0]);
		const glm::vec3 up = glm::cross(right, forward);

		pressedState.posx = cMouseState->posx;
		pressedState.posy = cMouseState->posy;
		
		if (input->IsKeyPressing(Input::KEY_W))
		{
			accel += forward;
		}
		if (input->IsKeyPressing(Input::KEY_S))
		{
			accel -= forward;
		}
		if (input->IsKeyPressing(Input::KEY_A))
		{
			if (input->IsKeyPressing(Input::KEY_X))
			{
				//r *= XMMatrixRotationRollPitchYaw(0, (XM_PI / 5) * frameTime, 0);
				//m_orientation = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(r);
			}
			else
			{
				accel -= right;
			}
		}
		if (input->IsKeyPressing(Input::KEY_D))
		{
			accel += right;
		}
		

		if (accel == s_stoppingVector)
		{
			//! There is no accel so damp
			if (m_moveSpeed != s_stoppingVector)
			{
				int a = 5;
			}
			auto dampingC = std::min((1.f / m_damping) * frameTime, 1.f);
			m_moveSpeed -= m_moveSpeed * dampingC;
		}
		else
		{
			if (input->IsKeyPressing(Input::KEY_SHIFT))
				m_isFast = true;

			if (m_isFast)
				accel *= m_fastCoef;

			m_moveSpeed += accel * m_acceleration * frameTime;
			const float maxSpeed = m_isFast ? m_maxSpeed * m_fastCoef : m_maxSpeed;
			if (glm::length(m_moveSpeed) > maxSpeed) m_moveSpeed = glm::normalize(m_moveSpeed) * maxSpeed;


		}
		m_pos += m_moveSpeed * frameTime;

		m_isFast = false;

		
	}
}