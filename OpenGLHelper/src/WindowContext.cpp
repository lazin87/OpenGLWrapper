#include "WindowContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Utilities.h"

namespace RL {

namespace GL {

namespace {

struct MouseCursorMovedCallback {};
struct MouseScrollCallback {};

template<typename ContextT, typename... CallbackArgs, typename TagT = DefaultTag>
auto makeCallback( ContextT* context
                 , void(ContextT::*callbackMethod)(CallbackArgs...)
                 , TagT tag = DefaultTag{}
                 ) -> void(*)(CallbackArgs...)
{
    using CallbackWrapper = CallableObjectToFunction<void(CallbackArgs...), TagT>;

    auto callback = std::bind( callbackMethod
                               , context
                               , std::placeholders::_1
                               , std::placeholders::_2
                               , std::placeholders::_3);
    CallbackWrapper::obj = callback;

    return CallbackWrapper::func;
}

} // namespace

struct WindowContextData
{
    WindowContext::WindowParams params;
    GLFWwindow *window;
};

WindowContext::WindowContext(const WindowContext::GLVersion &glVersion)
    : m_glVersion{glVersion}
    , m_windowContext{std::make_unique<WindowContextData>()}
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glVersion.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glVersion.minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_glVersion.isCoreProfile ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
}

WindowContext::~WindowContext()
{
    m_renderer.reset();
    glfwTerminate();
}

void WindowContext::createWindow(const WindowContext::WindowParams &windowParams)
{
    m_windowContext->params = windowParams;
    const auto &params = m_windowContext->params;

    m_windowContext->window = glfwCreateWindow(params.width, params.height, params.title.c_str(), nullptr, nullptr);
    if(m_windowContext->window == nullptr) throw std::logic_error{"Can not create window."};
    glfwMakeContextCurrent(m_windowContext->window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::logic_error{"Can not load GL procedures."};
}

void WindowContext::enableMouseHandling()
{
    if(m_windowContext->window == nullptr)
        return;

    auto w = m_windowContext->window;
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(w, makeCallback( this
                                            , &WindowContext::onCursorMoved
                                            , MouseCursorMovedCallback{}
                                            ) );
    glfwSetScrollCallback(w, makeCallback( this
                                         , &WindowContext::onScrollChanged
                                         , MouseScrollCallback{}
                                         ) );
}

void WindowContext::show()
{
    if(!m_renderer) return;

    if(m_windowContext->window == nullptr)
        createWindow({"Default Window", 800, 600, 0, 0});

    m_renderer->init();

    const auto window = m_windowContext->window;
    while(!glfwWindowShouldClose(window))
    {
        m_renderer->render();
        if(m_inputHandler)
        {
            m_inputHandler->processInput(window);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void WindowContext::onCursorMoved(GLFWwindow *w, double x, double y)
{
    if(!m_inputHandler) return;

     m_inputHandler->processMouseCursorMove(x, y);

}

void WindowContext::onScrollChanged(GLFWwindow *w, double x, double y)
{
    if(!m_inputHandler) return;

    m_inputHandler->processMouseScroll(x, y);
}

void WindowContext::set(std::shared_ptr<IRenderer> renderer)
{
    m_renderer = std::move(renderer);
}

void WindowContext::set(std::shared_ptr<IInputHandler> inputHandler)
{
    m_inputHandler = std::move(inputHandler);
}

} // namespace GL

} // namespace RL
