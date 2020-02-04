#pragma once

#include "IRenderer.h"

#include <map>
#include <memory>
#include <string>

namespace RL {

namespace GL {

struct WindowContextData;

class WindowContext
{
public:
    struct GLVersion
    {
        int major;
        int minor;
        bool isCoreProfile;
    };

    struct WindowParams
    {
        std::string title;
        int width;
        int height;
        /**
         * @brief X position of lower left corner. [pixels]
         */
        int x;
        /**
         * @brief Y position of lower left corner. [pixels]
         */
        int y;
    };

    WindowContext(const WindowContext&) = delete;
    WindowContext& operator=(const WindowContext&) = delete;
    WindowContext(WindowContext&&) = delete;
    WindowContext& operator=(WindowContext&&) = delete;

    WindowContext() = default;
    WindowContext(const GLVersion& glVersion);

    ~WindowContext();

    void createWindow(const WindowParams& windowParams); //throws std::logic_error in case of initialization failure.
    void show();

    void setRenderer(std::shared_ptr<IRenderer> renderer);

private:
    const GLVersion m_glVersion{3, 3, true};

    std::unique_ptr<WindowContextData> m_windowContext;
    std::shared_ptr<IRenderer> m_renderer;
};

} // namespace GL

} // namespace RL
