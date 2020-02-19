#include "FlyCameraControler.h"

#include "Camera.h"

#include <GLFW/glfw3.h>

#include <algorithm>

namespace RL {
namespace GL {


void FlyCameraControler::processInput(GLFWwindow *window)
{
    auto camera = m_camera.lock();
    if(!camera) return;

    updateLastFrameTime();
    const auto cameraSpeed = 2.5F * m_deltaTime;
    auto cameraParams = camera->getParams();

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraParams.position += cameraSpeed * cameraParams.front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraParams.position -= cameraSpeed * cameraParams.front;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraParams.position -= glm::normalize(glm::cross(cameraParams.front, cameraParams.up)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraParams.position += glm::normalize(glm::cross(cameraParams.front, cameraParams.up)) * cameraSpeed;
    }

    camera->setPosition(cameraParams.position);
}

void FlyCameraControler::processMouseCursorMove(const double x, const double y)
{
    auto camera = m_camera.lock();
    if(!camera) return;

    static bool isFirstCall = true;
    if(isFirstCall)
    {
        m_cursor.lastPosition.x = x;
        m_cursor.lastPosition.y = y;
        isFirstCall = false;
    }

    glm::vec2 positionDelta{
        (static_cast<float>(x) - m_cursor.lastPosition.x)
                , (-static_cast<float>(y) + m_cursor.lastPosition.y)
    };
    m_cursor.lastPosition.x = x;
    m_cursor.lastPosition.y = y;
    positionDelta *= m_cursor.sensitivity;

    m_lookDirection.yaw += positionDelta.x;
    m_lookDirection.pitch += positionDelta.y;
    m_lookDirection.pitch = std::clamp(m_lookDirection.pitch, -89.0F, 89.0F);

    const glm::vec3 direction {
        cos(glm::radians(m_lookDirection.yaw)) * cos(glm::radians(m_lookDirection.pitch)),
                sin(glm::radians(m_lookDirection.pitch)),
                sin(glm::radians(m_lookDirection.yaw)) * cos(glm::radians(m_lookDirection.pitch))
    };
    camera->setFront(direction);
}

void FlyCameraControler::processMouseScroll(double xOffset, double yOffset)
{
    auto camera = m_camera.lock();
    if(!camera) return;

    auto fov = camera->getOpticParams().fovDegrees - static_cast<float>(yOffset);
    fov = std::clamp(fov, 1.0F, 45.0F);
    camera->setFov(fov);
}

void FlyCameraControler::set(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void FlyCameraControler::updateLastFrameTime()
{
    const auto time = glfwGetTime();
    if(time != m_lastFrameTime)
    {
        m_deltaTime = time - m_lastFrameTime;
        m_lastFrameTime = time;
    }
}

} // namespace GL

} // namespace RL
