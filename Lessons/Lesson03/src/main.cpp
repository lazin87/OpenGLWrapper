#include "WindowContext.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "Texture2D.h"
#include "IRenderer.h"

#include <glad/glad.h>

using namespace RL::GL;

class Lesson03_textures : public IRenderer
{
    const std::vector<float> square_vertices = {
        // positions            // colors              // texture coords
         0.5F,  0.5F,  0.0F,    1.0F,  0.0F,  0.0F,    2.0F,  2.0F, // top right
         0.5F, -0.5F,  0.0F,    0.0F,  1.0F,  0.0F,    2.0F,  0.0F, // bottom right
        -0.5F, -0.5F,  0.0F,    0.0F,  0.0F,  1.0F,    0.0F,  0.0F, // bottom left
        -0.5F,  0.5F,  0.0F,    1.0F,  1.0F,  0.0F,    0.0F,  2.0F  // top left
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

        m_vao.setVericesData(square_vertices);
        m_vao.setIndices(indices);
        m_vao.setVertexAttribPointersConfig({0, GLSLType::vec3, false, 8 * sizeof(float), (void* )0});
        m_vao.setVertexAttribPointersConfig({1, GLSLType::vec3, false, 8 * sizeof(float), (void* )(3 * sizeof(float))});
        m_vao.setVertexAttribPointersConfig({2, GLSLType::vec2, false, 8 * sizeof(float), (void* )(6 * sizeof(float))});
        m_vao.build();

        m_texture1 = loadTexture2D("resources/container.jpg");
        m_texture2 = loadTexture2D("resources/awesomeface.png");
    }

    void render() override
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture1.getId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture2.getId());

        glBindVertexArray(m_vao.getId());
        glUseProgram(m_program.getId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUniform1i(glGetUniformLocation(m_program.getId(), "texture1"), 0);
        glUniform1i(glGetUniformLocation(m_program.getId(), "texture2"), 1);
    }

private:
    Program m_program;
    VertexArrayObject m_vao;
    Texture2D m_texture1;
    Texture2D m_texture2;
};

int main()
{
    WindowContext windowContext({3, 3, true});
    windowContext.createWindow({"Hello OpenGL", 800, 600, 0, 0});

    auto renderer = std::make_shared<Lesson03_textures>();
    windowContext.set(renderer);
    windowContext.show();

    return 0;
}
