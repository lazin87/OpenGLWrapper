#include "Camera.h"
#include "WindowContext.h"
#include "IRenderer.h"
#include "Program.h"
#include "FlyCameraControler.h"
#include "Texture2D.h"
#include "VertexArrayObject.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <array>

using namespace RL::GL;

static const std::vector<float> cubeVertices {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static const std::vector<float> planeVertices = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

class Lesson10: public IRenderer
{

public:
    void init() override
    {
        glEnable(GL_DEPTH_TEST);
        m_CubeVao.setVericesData(cubeVertices);
        m_CubeVao.setVertexAttribPointersConfig({ 0
                                                 , GLSLType::vec3
                                                 , false
                                                 , sizeof(float) * 5
                                                 , (void*) 0
                                                 });
        m_CubeVao.setVertexAttribPointersConfig({ 1
                                                 , GLSLType::vec2
                                                 , false
                                                 , sizeof(float) * 5
                                                 , (void*) (3 * sizeof(float))
                                                 });
        m_CubeVao.build();

        m_PlaneVao.setVericesData(planeVertices);
        m_PlaneVao.setVertexAttribPointersConfig({ 0
                                                 , GLSLType::vec3
                                                 , false
                                                 , sizeof(float) * 5
                                                 , (void*) 0
                                                 });
        m_PlaneVao.setVertexAttribPointersConfig({ 1
                                                  , GLSLType::vec2
                                                  , false
                                                  , sizeof(float) * 5
                                                  , (void*) (3 * sizeof(float))
                                                  });
        m_PlaneVao.build();

        m_program.addShader("resources/vs.vert");
        m_program.addShader("resources/fs.frag");
        m_program.build();

        m_metalTexture = loadTexture2D("resources/metal.png");
        m_marbleTexture = loadTexture2D("resources/marble.jpg");
    }

    void render() override
    {
        if(!m_camera) return;
        auto unitMatrix = glm::mat4(1.0F);
        const auto view = m_camera->getViewMatrix();
        const auto projection = m_camera->getProjectionMatrix();

        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_program.use();
        m_program.setViewMatrix(view);
        m_program.setProjectionMatrix(projection);
        m_program.setUniform("texture0", 0);

        m_CubeVao.use();
        m_marbleTexture.use(0);

        auto model = glm::translate(unitMatrix, m_cube1Pos);
        m_program.setModelMatrix(model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(unitMatrix, m_cube2Pos);
        m_program.setModelMatrix(model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        m_PlaneVao.use();
        m_metalTexture.use(0);
        m_program.setModelMatrix(unitMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void set(std::shared_ptr<Camera> camera)
    {
        m_camera = std::move(camera);
    }

private:
    VertexArrayObject m_CubeVao;
    VertexArrayObject m_PlaneVao;
    Texture2D m_metalTexture;
    Texture2D m_marbleTexture;
    Program m_program;
    std::shared_ptr<Camera> m_camera;

    glm::vec3 m_cube1Pos {-1.0F, 0.01F, -1.0F};
    glm::vec3 m_cube2Pos {2.0F, 0.01F, 0.0F};
};

int main()
{
    WindowContext window{ {3, 3, true} };
    window.createWindow( {"Lesson10", 800, 600, 0, 0} );
    window.enableMouseHandling();

    auto camera = std::make_shared<Camera>();
    camera->setPosition({0.0F, 1.0F, 10.0F});
    camera->setFront({0.0F, 0.0F, -1.0F});
    camera->setFov(45.0F);
    camera->setAspectRatio(800.0F / 600.0F);
    camera->setNearField(0.1F);
    camera->setFarField(100.0F);

    auto renderer = std::make_shared<Lesson10>();
    renderer->set(camera);
    window.set(renderer);

    auto inputControler = std::make_shared<FlyCameraControler>();
    inputControler->set(camera);
    window.set(inputControler);

    window.show();

    return 0;
}
