#include "pch.h"

#include "FPSCameraPositioner.h"
#include "Core/Game.h"
#include "Common/Utils/utils_imgui.h"
#include "DirectXTK/GeometricPrimitive.h"
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#define new DEBUG_NEW
#endif
using namespace	GNF::Common::Windowing::VC;
namespace GNF::Common::Camera::CameraPositioner
{
	//!: Strings
	constexpr static const char* g_str_x = "X";
	constexpr static const char* g_str_y = "Y";
	constexpr static const char* g_str_z = "Z";
	constexpr static const char* g_str_speed_x = "Speed X";
	constexpr static const char* g_str_speed_y = "Speed Y";
	constexpr static const char* g_str_speed_z = "Speed Z";
	constexpr static const char* g_str_speed = "Speed";
	constexpr static const char* g_str_max_speed = "Max Speed";
	constexpr static const char* g_str_acceleration = "Acceleration";
	constexpr static const char* g_str_fast_coef = "Fast Coef";
	constexpr static const char* g_str_dampling = "Dampling";
	constexpr static const char* g_str_rotation_x = "Pitch (X)";
	constexpr static const char* g_str_rotation_y = "Yaw (Y)";
	constexpr static const char* g_str_rotation_z = "Roll (Z)";
	constexpr static const char* g_str_mouse_sensivity = "Mouse Sensivity";
	constexpr static const char* g_str_mouse_epsilon = "Mouse Epsilon";
	static float g_epsilon = 0.0001f;

	constexpr static float g_gapBtw = 10.f;

	constexpr static std::string_view LongestStr = "Mouse Sensivity";
	

	constexpr static float MAX_PITCH_IN_RADIAN = DirectX::XMConvertToRadians(89.f);
	constexpr static float MIN_PITCH_IN_RADIAN = DirectX::XMConvertToRadians(-89.f);
	constexpr static float MAX_YAW_IN_RADIAN = DirectX::XMConvertToRadians(179.f);
	constexpr static float MIN_YAW_IN_RADIAN = DirectX::XMConvertToRadians(-179.f);


	using namespace DirectX;
	using namespace GNF::Common::Windowing::Keyboard;
	FPSCameraPositioner::FPSCameraPositioner(const DirectX::SimpleMath::Vector3& pos, float aspect, const DirectX::SimpleMath::Vector3& dir, float fov,float nearplane,float farplane) : m_pos(pos)
	{
		m_dir = dir;
		m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov, aspect, nearplane, farplane);
		m_model = SimpleMath::Matrix::Identity;
		m_view = SimpleMath::Quaternion::CreateFromRotationMatrix(SimpleMath::Matrix::CreateLookAt(m_pos, m_pos+m_dir,{0,1.f,0}));

		m_view = SimpleMath::Quaternion::Identity;

	}
	inline DirectX::SimpleMath::Matrix FPSCameraPositioner::GetViewMatrix()
	{
		auto rt = SimpleMath::Matrix::CreateFromQuaternion(m_view);
		auto tr = SimpleMath::Matrix::Identity;
		tr.Translation(-m_pos);
		return tr * rt;
	}
	void FPSCameraPositioner::UpdateUpVector()
	{
		auto viewC = GetViewMatrix();
		auto dir = viewC.Transpose().Forward();
		m_view = SimpleMath::Quaternion::CreateFromRotationMatrix(SimpleMath::Matrix::CreateLookAt(m_pos,m_pos+dir,m_up));
	}
	void FPSCameraPositioner::Update(const float deltaTime)
	{
		
		auto keyboard = Core::Game::GetInstance()->GetKeyboard();
		auto mouse = Core::Game::GetInstance()->GetMouse();
		auto accel = SimpleMath::Vector3(0, 0, 0);
		bool isMoved = false;
		if (keyboard->IsKeyReleased(KEY_F))
		{
			switch (mouse->GetMouseState())
			{
			case GNF::Common::Windowing::Mouse::VISIBLE:
			{
				mouse->SetMouseState(GNF::Common::Windowing::Mouse::DISABLE);
				mouse->CenterMouse();
				auto pos = mouse->GetMousePos();
				prevMousePosX = pos.x;
				prevMousePosY = pos.y;
				break;
			}
			case GNF::Common::Windowing::Mouse::DISABLE:
				mouse->SetMouseState(GNF::Common::Windowing::Mouse::VISIBLE);
				break;
			}
		}
		if (mouse->GetMouseState() == GNF::Common::Windowing::Mouse::DISABLE)
		{
			auto pos = mouse->GetMousePos();
			auto gpitch = (pos.x - prevMousePosX)  * m_mouseSensitivity;
			auto gyaw = (pos.y - prevMousePosY) * m_mouseSensitivity;
			
			if (abs(gpitch) < g_epsilon)
				gpitch = 0;
			if (abs(gyaw) < g_epsilon)
				gyaw = 0;

			mpitch += gpitch;
			myaw += gyaw;
			if (mpitch > MAX_PITCH_IN_RADIAN)
				mpitch = MAX_PITCH_IN_RADIAN;
			else if (mpitch < MIN_PITCH_IN_RADIAN)
				mpitch = MIN_PITCH_IN_RADIAN;
			if (myaw > MAX_YAW_IN_RADIAN)
				myaw = MAX_YAW_IN_RADIAN;
			else if (myaw < MIN_YAW_IN_RADIAN)
				myaw = MIN_YAW_IN_RADIAN;

			/*SimpleMath::Vector3 eulerAngles(myaw, mpitch, 0.f);
			SimpleMath::Matrix pitchMatrix = SimpleMath::Matrix::CreateRotationX(eulerAngles.x);
			SimpleMath::Matrix yawMatrix = SimpleMath::Matrix::CreateRotationY(eulerAngles.y);
			SimpleMath::Matrix rollMatrix = SimpleMath::Matrix::CreateRotationZ(eulerAngles.z);

			SimpleMath::Matrix rotationMatrix = rollMatrix * yawMatrix * pitchMatrix;*/

			m_view = SimpleMath::Quaternion::CreateFromYawPitchRoll(mpitch,myaw,0.f);

			m_view.Normalize();
			
			UpdateUpVector();

			mouse->CenterMouse();
		}
		auto viewAsMatrix = SimpleMath::Matrix::CreateFromQuaternion(m_view).Transpose();

		if (keyboard->IsKeyPressed(KEY_A))
		{
			accel += viewAsMatrix.Left();
			isMoved = true;
		}
		else if (keyboard->IsKeyPressed(KEY_D))
		{
			accel += viewAsMatrix.Right();
			isMoved = true;
		}
		if (keyboard->IsKeyPressed(KEY_W))
		{
			accel += viewAsMatrix.Forward();
			isMoved = true;
		}
		else if (keyboard->IsKeyPressed(KEY_S))
		{
			accel += viewAsMatrix.Backward();
			isMoved = true;
		}

		if (m_camSpeed.Length() != 0.f)
		{
			auto dampingC = std::min(1.f,(1.f/m_damping)*deltaTime );
			m_camSpeed -= dampingC * m_camSpeed ;
		}
		//!: Moved
		if (isMoved)
		{
			bool isFast = false;
			if (keyboard->IsKeyPressed(KEY_SHIFT))
				isFast = true;
			if(isFast)
				accel *= m_fastCoef;
			m_camSpeed += accel * m_acceleration *deltaTime;
			const float maxSpeed = isFast ? m_maxSpeed * m_fastCoef : m_maxSpeed;
			if (m_camSpeed.Length() > maxSpeed)
			{
				m_camSpeed.Normalize();
				m_camSpeed *= maxSpeed;
			}
		}

		m_pos+=m_camSpeed* deltaTime;
	}
	void FPSCameraPositioner::ReCalculateView()
	{
		auto qt = SimpleMath::Quaternion::CreateFromYawPitchRoll(SimpleMath::Vector3(DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(roll), DirectX::XMConvertToRadians(yaw) ));
		auto dir =SimpleMath::Vector3::Transform(m_dir, qt);
		auto up = SimpleMath::Vector3::Transform(m_up, qt);
		
		m_view = SimpleMath::Quaternion::CreateFromRotationMatrix(SimpleMath::Matrix::CreateLookAt(m_pos, m_pos + dir, up));
		
	}

	void FPSCameraPositioner::RenderGui()
	{

		//X: Should change at the future
		str_for_pos_x = std::to_string(m_pos.x);
		str_for_pos_y = std::to_string(m_pos.y);
		str_for_pos_z = std::to_string(m_pos.z);
		str_for_pos_speed_x = std::to_string(m_camSpeed.x);
		str_for_pos_speed_y = std::to_string(m_camSpeed.y);
		str_for_pos_speed_z = std::to_string(m_camSpeed.z);
		str_for_pos_speed_length = std::to_string(m_camSpeed.Length());
		str_for_rotation_x = std::to_string(XMConvertToDegrees(mpitch));
		str_for_rotation_y = std::to_string(XMConvertToDegrees(myaw));
		str_for_rotation_z = std::to_string(0.f);

		//!: No / 2 for space between left border
		float twidth = ImGui::CalcTextSize(LongestStr.data()).x;

		Utils::ImGui_Header2("Translation");
		Utils::ImGui_LeftLabeledInputRO(g_str_x, (char*)str_for_pos_x.c_str(), str_for_pos_x.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_y, (char*)str_for_pos_y.c_str(), str_for_pos_y.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_z, (char*)str_for_pos_z.c_str(), str_for_pos_z.size() - 1, twidth, g_gapBtw);
	
		Utils::ImGui_Header2("Speed");
	
		Utils::ImGui_LeftLabeledInputRO(g_str_speed_x, (char*)str_for_pos_speed_x.c_str(), str_for_pos_speed_x.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_speed_y, (char*)str_for_pos_speed_y.c_str(), str_for_pos_speed_y.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_speed_z, (char*)str_for_pos_speed_z.c_str(), str_for_pos_speed_z.size() - 1, twidth, g_gapBtw);
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		Utils::ImGui_LeftLabeledInputRO(g_str_speed, (char*)str_for_pos_speed_length.c_str(), str_for_pos_speed_length.size() - 1, twidth, g_gapBtw);
		
		Utils::ImGui_Header2("Props");

		Utils::ImGui_LeftLabeledSlideFloat(g_str_max_speed, &m_maxSpeed, 5.f, 15.f, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledSlideFloat(g_str_acceleration, &m_acceleration, 30.f, 500.f, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledSlideFloat(g_str_fast_coef, &m_fastCoef, 2.f, 30.f, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledSlideFloat(g_str_dampling, &m_damping, 0.1f, 0.5f, twidth, g_gapBtw);
	
		Utils::ImGui_Header2("Rotation");


		Utils::ImGui_LeftLabeledInputRO(g_str_rotation_x, (char*)str_for_rotation_x.c_str(), str_for_rotation_x.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_rotation_y, (char*)str_for_rotation_y.c_str(), str_for_rotation_y.size() - 1, twidth, g_gapBtw);
		Utils::ImGui_LeftLabeledInputRO(g_str_rotation_z, (char*)str_for_rotation_z.c_str(), str_for_rotation_z.size() - 1, twidth, g_gapBtw);
		
		Utils::ImGui_Header2("Props");

		Utils::ImGui_LeftLabeledSlideFloat(g_str_mouse_sensivity, &m_mouseSensitivity, 0.001f, 0.01f, twidth, g_gapBtw,"%.4f");
		ImGui::Spacing();
		ImGui::Spacing();
		Utils::ImGui_LeftLabeledSlideFloat(g_str_mouse_epsilon, &g_epsilon, 0.00001f, 0.1f, twidth, g_gapBtw,"%.6f");

	}

	const DirectX::SimpleMath::Matrix& FPSCameraPositioner::GetViewProjection()
	{

		return (GetViewMatrix() * m_proj);
	}
}