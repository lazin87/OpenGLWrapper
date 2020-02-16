#pragma once

class GLFWwindow;

namespace RL {

namespace GL {

class IInputHandler
{
public:
    virtual ~IInputHandler() = default;

    virtual void processInput(GLFWwindow *window) = 0;
    virtual void processMouseCursorMove(const double /* x */, const double /* y */) {};
    virtual void processMouseScroll(const double /* x */, const double /* y */) {};
};

} // namespace GL

} // namespace RL
