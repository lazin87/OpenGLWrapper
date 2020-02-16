#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RL {

namespace GL {

namespace
{

    template<typename ValueT>
    void setOnChange(ValueT& oldVal, const ValueT& newVal, bool& dirtyFlag)
    {
        if(newVal != oldVal)
        {
            dirtyFlag = true;
            oldVal = newVal;
        }
    }

} // namespace

void Camera::setPosition(const glm::vec3 &p)
{
    setOnChange(m_params.position, p, m_viewDirtyFlag);
}

void Camera::setFront(const glm::vec3 &f)
{
    setOnChange(m_params.front, f, m_viewDirtyFlag);
}

void Camera::setUpVector(const glm::vec3 &u)
{
    setOnChange(m_params.up, u, m_viewDirtyFlag);
}

CameraParams Camera::getParams() const
{
    return m_params;
}

void Camera::setFov(const float degrees)
{
    setOnChange(m_opticParams.fovDegrees, degrees, m_projectionDirtyFlag);
}

void Camera::setAspectRatio(const float a)
{
    setOnChange(m_opticParams.aspectRatio, a, m_projectionDirtyFlag);
}

void Camera::setNearField(const float n)
{
    setOnChange(m_opticParams.near, n, m_projectionDirtyFlag);
}

void Camera::setFarField(const float f)
{
    setOnChange(m_opticParams.far, f, m_projectionDirtyFlag);
}

CameraOpticParams Camera::getOpticParams() const
{
    return m_opticParams;
}

glm::mat4 Camera::getViewMatrix() const
{
    if(m_viewDirtyFlag)
    {
        m_viewMatrix = glm::lookAt( m_params.position
                                  , m_params.position + m_params.front
                                  , m_params.up);
        m_viewDirtyFlag = false;
    }

    return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    if(m_projectionDirtyFlag)
    {
        m_projectionMatrix = glm::perspective( glm::radians(m_opticParams.fovDegrees)
                                             , m_opticParams.aspectRatio
                                             , m_opticParams.near
                                             , m_opticParams.far);
        m_projectionDirtyFlag = false;
    }

    return m_projectionMatrix;
}

} // namespace GL

} // namespace RL
