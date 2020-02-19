#pragma once

#include "IInputHandler.h"

#include <glm/glm.hpp>

#include <memory>

namespace RL {
namespace GL {

class Camera;

struct EulerAngles
{
    float pitch;
    float yaw;
    float roll;
};

class FlyCameraControler: public IInputHandler
{
    struct MouseCursor
    {
        glm::vec2 lastPosition;
        float sensitivity;
    };

public:
    void processInput(GLFWwindow *window) override;
    void processMouseCursorMove(const double x, const double y) override;
    void processMouseScroll(double xOffset, double yOffset) override;
    void set(std::shared_ptr<Camera> camera);

private:
    void updateLastFrameTime();

    std::weak_ptr<Camera> m_camera;
    float m_deltaTime {0.0F};
    float m_lastFrameTime {0.0F};
    MouseCursor m_cursor {{400.0F, 300.0F}, 0.05F};
    EulerAngles m_lookDirection {0.0F, -90.0F, 0.0F};

};

} // namespace GL

} // namespace RL

