#include "Camera.h"
#include "WindowContext.h"
#include "IRenderer.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "FlyCameraControler.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace RL::GL;

class Lesson06: public IRenderer
{

public:
    const std::vector<float> cube_vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
        -0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

        -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

        -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F,  0.5F, -0.5F,  0.0F, 1.0F
    };

    void init() override
    {
        glEnable(GL_DEPTH_TEST);

        m_programMain.addShader("resources/vs.vert");
        m_programMain.addShader("resources/fs.frag");
        m_programMain.build();

        m_programLight.addShader("resources/vs.vert");
        m_programLight.addShader("resources/fs_light.frag");
        m_programLight.build();

        m_vaoCube.setVericesData(cube_vertices);
        m_vaoCube.setVertexAttribPointersConfig({ 0
                                                , GLSLType::vec3
                                                , false
                                                , 5 * sizeof(float)
                                                , (void*) 0
                                                });
        m_vaoCube.build();

        m_programMain.use();
        const auto unityMatrix = glm::mat4{1.0F};
        m_programMain.setModelMatrix(unityMatrix);
        m_programMain.setViewMatrix(unityMatrix);
        m_programMain.setProjectionMatrix(unityMatrix);
        m_programMain.setUniform("lightColor", {1.0F, 0.5F, 0.31F});
        m_programMain.setUniform("objectColor", {1.0F, 1.0F, 1.0F});

        m_programLight.use();
        auto lightTransform = glm::translate(unityMatrix, m_lightPosition);
        lightTransform = glm::scale(lightTransform, glm::vec3(0.2F));
        m_programLight.setModelMatrix(lightTransform);
        m_programLight.setViewMatrix(unityMatrix);
        m_programLight.setProjectionMatrix(unityMatrix);
    }

    void render() override
    {
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(m_vaoCube.getId());

        if(m_camera)
        {
            m_programMain.use();
            m_programMain.setViewMatrix(m_camera->getViewMatrix());
            m_programMain.setProjectionMatrix(m_camera->getProjectionMatrix());

            m_programLight.use();
            m_programLight.setViewMatrix(m_camera->getViewMatrix());
            m_programLight.setProjectionMatrix(m_camera->getProjectionMatrix());
        }

        m_programMain.use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        m_programLight.use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
    }

    void set(std::shared_ptr<Camera> c)
    {
        m_camera = std::move(c);
    }

private:
    Program m_programMain;
    Program m_programLight;
    VertexArrayObject m_vaoCube;
    std::shared_ptr<Camera> m_camera;

    glm::vec3 m_lightPosition {1.2F, 1.0F, 2.0F};
};

int main()
{
    WindowContext window{ {3, 3, true} };
    window.createWindow( {"Lesson06", 800, 600, 0, 0} );
    window.enableMouseHandling();

    auto camera = std::make_shared<Camera>();
    camera->setPosition({0.0F, 0.0F, 3.0F});
    camera->setFront({0.0F, 0.0F, -1.0F});
    camera->setFov(45.0F);
    camera->setAspectRatio(800.0F / 600.0F);
    camera->setNearField(0.1F);
    camera->setFarField(100.0F);

    auto renderer = std::make_shared<Lesson06>();
    renderer->set(camera);
    window.set(renderer);

    auto inputControler = std::make_shared<FlyCameraControler>();
    inputControler->set(camera);
    window.set(inputControler);

    window.show();

    return 0;
}
