#include "WindowContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace RL {

namespace GL {

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

void WindowContext::show()
{
    if(m_windowContext->window == nullptr)
        createWindow({"Default Window", 800, 600, 0, 0});

    if(m_renderer) m_renderer->init();

    const auto window = m_windowContext->window;
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.5F, 0.1F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        if(m_renderer) m_renderer->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void WindowContext::setRenderer(std::shared_ptr<IRenderer> renderer)
{
    m_renderer = std::move(renderer);
}

} // namespace GL

} // namespace RL
