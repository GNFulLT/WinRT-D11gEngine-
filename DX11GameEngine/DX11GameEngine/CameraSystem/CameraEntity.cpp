
#include "CameraEntity.h"
#include "Game/GameMain.h"
#include "InputSystem/VirtualKeyCodes.h"

namespace GNF::Camera
{
	CameraEntity::CameraEntity()
	{

	}
	void CameraEntity::Update(const float& frameTime)
	{
		auto inputs = Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>();
		if(inputs->IsKeyPressing(Input::KEY_A) || inputs->IsKeyPressing(Input::KEY_LEFT))
		{

			if (inputs->IsKeyPressing(Input::KEY_X))
			{
				camView *= XMMatrixRotationRollPitchYaw(0, (XM_PI / 5) * frameTime, 0);
				UpdateWVP();
			}
			else
			{
				camView *= XMMatrixTranslation(2.f * frameTime, 0, 0);
				UpdateWVP();

			}
		}
		if (inputs->IsKeyPressing(Input::KEY_D) || inputs->IsKeyPressing(Input::KEY_RIGHT))
		{
			if (inputs->IsKeyPressing(Input::KEY_X))
			{
				camView *= XMMatrixRotationRollPitchYaw(0, (-XM_PI / 5) * frameTime, 0);
				UpdateWVP();

			}
			else
			{
				camView *= XMMatrixTranslation(-2.f * frameTime, 0, 0);
				UpdateWVP();

			}
		}
		if (inputs->IsKeyPressing(Input::KEY_W) || inputs->IsKeyPressing(Input::KEY_UP))
		{
			if (inputs->IsKeyPressing(Input::KEY_X))
			{
				camView *= XMMatrixRotationRollPitchYaw((XM_PI / 5) * frameTime, 0, 0);
				UpdateWVP();

			}
			else
			{
				camView *= XMMatrixTranslation(0, 0, -2.f * frameTime);

				UpdateWVP();

			}
		}
		if (inputs->IsKeyPressing(Input::KEY_S) || inputs->IsKeyPressing(Input::KEY_DOWN))
		{
			if (inputs->IsKeyPressing(Input::KEY_X))
			{
				camView *= XMMatrixRotationRollPitchYaw((-XM_PI / 5) * frameTime, 0, 0);
				UpdateWVP();
			}
			else
			{
				camView *= XMMatrixTranslation(0, 0, 2.f * frameTime);
				UpdateWVP();
			}
		}
	}
	void CameraEntity::UpdateWVP()
	{
		WVP = World * camView * camProjection;
		cbPerObj.WVP = XMMatrixTranspose(WVP);
		constBuffer->Update(&cbPerObj);
		constBuffer->Bind();
	}

	void CameraEntity::InitResources()
	{
		auto resources = Game::GameMain::GetInstance()->GetMainWindow()->GetDeviceResources();

		//! Set Camera Positions

		camPosition = XMVectorSet(0.0f, 0.0f, -1.5f, 0.0f);
		camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		//! Creating camera matrix with lookat
		camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);


		//! Creating Projection
		//! 
		camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)resources->AspectRatio(), 1.0f, 1000.0f);

		//! Reset Identity and create WVP
		World = XMMatrixIdentity();

		WVP = World * camView * camProjection;
		cbPerObj.WVP = XMMatrixTranspose(WVP);
		constBuffer.reset(new Common::Bindable::ConstBufferBindable(resources, &cbPerObj, sizeof(cbPerObject)));
		constBuffer->Bind();
	}
}