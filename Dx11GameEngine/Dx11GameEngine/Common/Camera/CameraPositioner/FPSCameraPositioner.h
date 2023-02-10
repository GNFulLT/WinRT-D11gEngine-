#pragma once

#include "ICameraPositioner.h"
#include <DirectXMath.h>
#include "glm/glm.hpp"
#include <glm/gtx/euler_angles.hpp>
namespace GNF::Common::Camera::CameraPositioner
{
	constexpr static DirectX::SimpleMath::Vector3 def = DirectX::SimpleMath::Vector3(0.f, 0.f,-1.f);
	class FPSCameraPositioner final : public ICameraPositioner
	{
	public:
		FPSCameraPositioner(const DirectX::SimpleMath::Vector3& pos, float aspect = 0.75f, const DirectX::SimpleMath::Vector3& target = def, float fov = DirectX::XM_PI / 4.f, float nearplane = 0.1f, float farplane = 1000.f);
		
		void Update(const float deltaTime) override;
		const DirectX::SimpleMath::Matrix& GetViewProjection() override;
		void RenderGui() override;
		const DirectX::SimpleMath::Vector3& GetPos() override;
	private:
		void ReCalculateView();
	private:
		DirectX::SimpleMath::Vector3 m_pos;
		DirectX::SimpleMath::Vector3 m_dir;
		DirectX::SimpleMath::Vector3 m_up = { 0,1.f,0 };
		
		DirectX::SimpleMath::Matrix m_proj;
		DirectX::SimpleMath::Quaternion m_view;
		DirectX::SimpleMath::Matrix m_model;

		float mpitch = 0.f;
		float myaw = 0.f;

		DirectX::SimpleMath::Vector3 m_camSpeed = {0,0,0};
	

		float m_damping = 0.1f;
		float m_fastCoef = 2.f;
		float m_acceleration = 150.f;
		float m_maxSpeed = 10.0f;
		float m_mouseSensitivity = 0.005f;


		int prevMousePosX = 0;
		int prevMousePosY = 0;

		//! Y
		float roll  = 0.f;;
		//! X
		float pitch = 0.f;
		//! Z
		float yaw = 0.f;
	private:
		void UpdateUpVector();
		inline DirectX::SimpleMath::Matrix GetViewMatrix();
		
	private:
		std::string str_for_pos_x;
		std::string str_for_pos_y;
		std::string str_for_pos_z;
		
		std::string str_for_pos_speed_x;
		std::string str_for_pos_speed_y;
		std::string str_for_pos_speed_z;
		std::string str_for_pos_speed_length;

		std::string str_for_rotation_x;
		std::string str_for_rotation_y;
		std::string str_for_rotation_z;
	};
}