#pragma once


#include "Common/System/ISystem.h"
#include "ICameraPositioner.h"

namespace GNF::Camera
{
	class CameraSystem final : public Common::ISystem
	{
		public:
			explicit CameraSystem(ICameraPositioner& positioner) : p_positioner(positioner)
			{}
			
			CameraSystem(const CameraSystem&) = delete;
			CameraSystem& operator=(const CameraSystem&) = delete;

			virtual int SystemID() override
			{
				return 5;
			}


			glm::mat4 GetViewMatrix()
			{
				return p_positioner.GetViewMatrix();
			}
			glm::vec3 GetPosition()
			{
				return p_positioner.GetPosition();
			}
			glm::mat4 GetViewProjMatrix()
			{
				return p_positioner.GetViewProjMatrix();
			}
			glm::mat4 GetWorldViewProjMatrix(const glm::mat4& world)
			{
				return p_positioner.GetWorldViewProjMatrix(world);

			}
		private:
			ICameraPositioner& p_positioner;
	};
}