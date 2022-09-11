#pragma once
#include "CameraTestRenderer.h"
#include <Common/VertexTypes.h>
#include "InputSystem/VirtualKeyCodes.h"
#include "Game/GameMain.h"
#include "InputSystem/IInputSystem.h"
#include "InputSystem/VirtualKeyCodes.h"
#include "Common/Timer/ITimerSystem.h"
using namespace GNF::Common;

namespace GNF::Graphic
{
	void CameraTestRenderer::UpdateThings()
	{
		/*if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_A))
		{
			if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_X))
			{
				TurnLeft();
				Update();
			}
			else
			{
				Left();
				Update();

			}
		}
		if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_D))
		{
			if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_X))
			{
				TurnRight();
				Update();

			}
			else
			{
				Right();
				Update();

			}
		}
		if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_W))
		{
			if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_X))
			{
				TurnUp();
				Update();

			}
			else
			{
				Up();
				Update();

			}
		}
		if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_S))
		{
			if (Game::GameMain::GetInstance()->GetSystem<Input::IInputSystem>()->IsKeyPressing(Input::KEY_X))
			{
				TurnDown();
				Update();
			}
			else
			{
				Down();
				Update();
			}
		}*/

	}

	void CameraTestRenderer::Left()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixTranslation(2.f*frameTime, 0, 0);
	}
	void CameraTestRenderer::Right()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixTranslation(-2.f*frameTime, 0, 0);

	}
	void CameraTestRenderer::Up()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixTranslation(0, 0, -2.f*frameTime);

	}
	void CameraTestRenderer::Down()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();
		camView *= XMMatrixTranslation(0, 0, 2.f*frameTime);

	}
	void CameraTestRenderer::TurnLeft()
	{
		//auto left = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixRotationRollPitchYaw(0, (XM_PI / 5)*frameTime, 0);
		
		//camView *= left;
	}
	
	void CameraTestRenderer::TurnRight()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixRotationRollPitchYaw(0, ( - XM_PI / 5)*frameTime, 0);

	}

	void CameraTestRenderer::TurnUp()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixRotationRollPitchYaw((XM_PI / 5)*frameTime, 0, 0);

	}
	void CameraTestRenderer::TurnDown()
	{
		float frameTime = Game::GameMain::GetInstance()->GetFrameTime();

		camView *= XMMatrixRotationRollPitchYaw((- XM_PI / 5)*frameTime, 0, 0);

	}

	void CameraTestRenderer::Update()
	{
		WVP = World * camView * camProjection;
		cbPerObj.WVP = XMMatrixTranspose(WVP);
	}


	void CameraTestRenderer::InitResources()
	{
		

		


	}

	void CameraTestRenderer::Render()
	{
		auto m_d3d_device = pParentDevice->GetD3DDevice();
		auto m_d3d_deviceContext = pParentDevice->GetD3DContext();

	

		int sizeOfInd = 12;
		DWORD indices[] = {
						0, 1, 2,
						0, 2, 3,

						4,5,6,
						4,6,7
		};

		ComPtr<ID3D11Buffer> squareIndexBuffer;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * sizeOfInd;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		m_d3d_device->CreateBuffer(&indexBufferDesc, &iinitData, squareIndexBuffer.GetAddressOf());

		//Set the vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		vertexBuffer->Bind();

		m_d3d_deviceContext->IASetIndexBuffer(squareIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		//Set the Input Layout
		m_d3d_deviceContext->IASetInputLayout(vertLayout.Get());

		//Set Primitive Topology
		m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		m_d3d_deviceContext->DrawIndexed(sizeOfInd, 0, 0);
	}
}

	
