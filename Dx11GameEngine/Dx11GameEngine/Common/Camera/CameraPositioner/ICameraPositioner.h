#pragma once

#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>

namespace GNF::Common::Camera::CameraPositioner
{
	class ICameraPositioner
	{
	public:
		virtual ~ICameraPositioner() = default;

		virtual void Update(const float deltaTime) = 0;
		virtual const DirectX::SimpleMath::Matrix& GetViewProjection() = 0;
		virtual void RenderGui() = 0;
	private:
		
	};
}