#include "WindowContext.h"

#include "lessons/lesson02_triangle.cpp"
#include "lessons/lesson03_textures.cpp"

int main()
{
    WindowContext windowContext({3, 3, true});
    windowContext.createWindow({"Hello OpenGL", 800, 600, 0, 0});

    auto renderer = std::make_shared<HelloRenderer>();
    windowContext.setRenderer(renderer);
    windowContext.show();

    return 0;
}
