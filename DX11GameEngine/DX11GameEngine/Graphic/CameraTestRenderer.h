#pragma once
#include "IRenderer3.h"
#include "Common/Device/IDeviceResources.h"
#include <DirectXMath.h>
#include "Common/Bindable/ConstBufferBindable.h"
#include <memory>
#include "Common/Bindable/VertexBufferBindable.h"

using namespace DirectX;
namespace GNF::Graphic
{
	struct cbPerObject
	{
		XMMATRIX  WVP;
	};


	class CameraTestRenderer : public IRenderer3
	{
		public:
			CameraTestRenderer(Common::Device::IDeviceResources* res) : pParentDevice(res){}

			~CameraTestRenderer()
			{
				int a = 5;
			}
			virtual void Render() override;
			virtual void InitResources() override;
			virtual Common::Device::IDeviceResources* GetResources() override
			{
				return pParentDevice;
			}

			void Left();
			void Right();
			void Up();
			void Down();
			void TurnLeft();
			void TurnRight();
			void TurnUp();
			void TurnDown();
			virtual void UpdateThings() override;

			
		private:
			ComPtr<ID3D11InputLayout> vertLayout;
			Common::Device::IDeviceResources* pParentDevice;
			cbPerObject cbPerObj;
			std::unique_ptr<Common::Bindable::VertexBufferBindable> vertexBuffer;
			inline void Update();
		private:
			//! World View Matrix
			XMMATRIX WVP;
			XMMATRIX World;
			XMMATRIX camView;
			XMMATRIX camProjection;

			//! For Camera Position
			XMVECTOR camPosition;
			XMVECTOR camTarget;
			XMVECTOR camUp;

			//! For Camera Rotation Matrix
			float roll;
			float pitch;
			float yaw;

	};
}