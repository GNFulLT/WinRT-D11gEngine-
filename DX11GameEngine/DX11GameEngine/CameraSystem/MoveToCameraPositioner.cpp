#include "MoveToCameraPositioner.h"
#include "FPSCameraPositioner.h"

glm::mat4 GNF::Camera::MoveToCameraPositioner::GetViewMatrix()
{
    return m_currentTransform;
}

glm::vec3 GNF::Camera::MoveToCameraPositioner::GetPosition()
{
    return m_posCurrent;
}

void GNF::Camera::MoveToCameraPositioner::Update(const float frameTime)
{
    m_posCurrent = m_linearDamping * frameTime * (m_posDesired - m_posCurrent);
    
    //! Fix the bug -180 , 180 deg
    m_angleCurrent = clipAngles(m_angleCurrent);
    m_angleDesired = clipAngles(m_angleDesired);

    m_angleCurrent -= angleDelta(m_angleCurrent, m_angleDesired) * m_eularDamping * frameTime;

    const glm::vec3 a = glm::radians(m_angleCurrent);

    m_currentTransform = glm::translate(glm::yawPitchRoll(a.y, a.x, a.z), -m_posCurrent);

}

void GNF::Camera::MoveToCameraPositioner::InitResources()
{
}

glm::mat4 GNF::Camera::MoveToCameraPositioner::GetViewProjMatrix()
{
    return  FPSCameraPositioner::m_proj * GetViewMatrix();
}

glm::mat4 GNF::Camera::MoveToCameraPositioner::GetWorldViewProjMatrix(const glm::mat4& world)
{
    return FPSCameraPositioner::m_proj * GetViewMatrix() * world;
}
