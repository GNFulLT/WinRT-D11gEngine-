#pragma once

#include "ICameraPositioner.h"

namespace GNF::Camera
{
	class MoveToCameraPositioner final : public ICameraPositioner
	{
		public:
			MoveToCameraPositioner() = default;

			MoveToCameraPositioner(const glm::vec3& pos, const glm::vec3& angles)
				: m_posCurrent(pos)
				, m_posDesired(pos)
				, m_angleCurrent(angles)
				, m_angleDesired(angles)
			{}

			virtual glm::mat4 GetViewMatrix() override;
			virtual glm::vec3 GetPosition() override;
			virtual void Update(const float frameTime) override;
			virtual void InitResources() override;
			virtual  glm::mat4 GetViewProjMatrix() override;
			virtual  glm::mat4 GetWorldViewProjMatrix(const glm::mat4& world) override;

			//! Helper Functions
		private:

			//! Prevent -180 +180 rotate bugs
			static inline float clipAngle(float d)
			{
				if (d < -180.0f) return d + 360.0f;
				if (d > +180.0f) return d - 360.f;
				return d;
			}

			//! Multiple Clip Angle
			static inline glm::vec3 clipAngles(const glm::vec3& angles)
			{
				return glm::vec3(
					std::fmod(angles.x, 360.0f),
					std::fmod(angles.y, 360.0f),
					std::fmod(angles.z, 360.0f)
				);
			}

			//! All Delta between two angle
			static inline glm::vec3 angleDelta(const glm::vec3& anglesCurrent, const glm::vec3& anglesDesired)
			{
				const glm::vec3 d = clipAngles(anglesCurrent) - clipAngles(anglesDesired);
				return glm::vec3(clipAngle(d.x), clipAngle(d.y), clipAngle(d.z));
			}

		private:
			float m_linearDamping = 10.f;
			glm::vec3 m_eularDamping = glm::vec3(5.f,5.f,5.f);


			//! Vec And Matrixes
		private:
			glm::vec3 m_posCurrent;
			glm::vec3 m_posDesired;
			glm::vec3 m_angleCurrent;
			glm::vec3 m_angleDesired;


			glm::mat4 m_currentTransform;
	
	
	};
}