#pragma once


#include "EntitySystem/IEntity.h"

#include "glm/glm.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace GNF::Camera
{
	class ICameraPositioner : public Entity::IEntity
	{
		public:
			virtual ~ICameraPositioner() = default;
			virtual glm::mat4 GetViewMatrix() = 0;
			virtual glm::vec3 GetPosition() = 0;
			virtual void Update(const float frameTime) = 0;
			virtual void InitResources() = 0;
			virtual  glm::mat4 GetViewProjMatrix() = 0;
			virtual  glm::mat4 GetWorldViewProjMatrix(const glm::mat4& world) = 0;

		private:
	};
}