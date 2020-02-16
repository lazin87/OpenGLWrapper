#include "WindowContext.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "IRenderer.h"

#include <glad/glad.h>

using namespace RL::GL;

class Lesson02_triangle : public IRenderer
{
    const std::vector<float> triangle = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };
    const std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

public:
    void init() override
    {
        m_program.addShader("resources/vs.vert");
        m_program.addShader("resources/fs.frag");
        m_program.build();

        m_vao.setVericesData(triangle);
        m_vao.setIndices(indices);
        m_vao.setVertexAttribPointersConfig({0, GLSLType::vec3, false, 3 * sizeof(float), (void* )0});
        m_vao.build();
    }

    void render() override
    {
        glUseProgram(m_program.getId());
        glBindVertexArray(m_vao.getId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    Program m_program;
    VertexArrayObject m_vao;
};

int main()
{
    WindowContext windowContext({3, 3, true});
    windowContext.createWindow({"Hello OpenGL", 800, 600, 0, 0});

    auto renderer = std::make_shared<Lesson02_triangle>();
    windowContext.set(renderer);
    windowContext.show();

    return 0;
}
