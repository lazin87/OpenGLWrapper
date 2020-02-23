#include "Camera.h"
#include "WindowContext.h"
#include "IRenderer.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "FlyCameraControler.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <vector>

using namespace RL::GL;

class Lesson06: public IRenderer
{

public:
    const std::vector<float> cube_vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

        m_programMaterial.addShader("resources/vs.vert");
        m_programMaterial.addShader("resources/fs_material.frag");
        m_programMaterial.build();

        m_vaoCube.setVericesData(cube_vertices);
        m_vaoCube.setVertexAttribPointersConfig({ 0
                                                , GLSLType::vec3
                                                , false
                                                , 6 * sizeof(float)
                                                , (void*) 0
                                                });
        m_vaoCube.setVertexAttribPointersConfig({ 1
                                                , GLSLType::vec3
                                                , false
                                                , 6 * sizeof(float)
                                                , (void*) (3 * sizeof(float))
                                                });

        m_vaoCube.build();

        const auto unityMatrix = glm::mat4{1.0F};
        m_programMain.use();
        m_programMain.setModelMatrix(m_objectModelMatrix);
        m_programMain.setViewMatrix(unityMatrix);
        m_programMain.setProjectionMatrix(unityMatrix);
        m_programMain.setNormalMatrix(unityMatrix);
        m_programMain.setUniform("objectColor", {1.0F, 1.0F, 1.0F});
        m_programMain.setUniform("lightColor", {1.0F, 0.5F, 0.31F});
        m_programMain.setUniform("lightPos", m_lightPosition);

        m_programMaterial.use();
        auto materialObjTransform = glm::translate(unityMatrix, {1.2F, 0.0F, 2.0F});
        m_programMaterial.setModelMatrix(materialObjTransform);
        m_programMaterial.setViewMatrix(unityMatrix);
        m_programMaterial.setProjectionMatrix(unityMatrix);
        m_programMaterial.setNormalMatrix(unityMatrix);
        m_programMaterial.setUniform("material.ambient", {1.0F, 0.5F, 0.31F});
        m_programMaterial.setUniform("material.diffuse", {1.0F, 0.5F, 0.31F});
        m_programMaterial.setUniform("material.specular", {0.5F, 0.5F, 0.5F});
        m_programMaterial.setUniform("material.shininess", 32.0F);
        m_programMaterial.setUniform("light.ambient", {0.2F, 0.2F, 0.2F});
        m_programMaterial.setUniform("light.diffuse", {0.5F, 0.5F, 0.5F});
        m_programMaterial.setUniform("light.specular", {1.0F, 1.0F, 1.0F});
        m_programMaterial.setUniform("light.position", m_lightPosition);



        m_programLight.use();
        auto lightTransform = glm::translate(unityMatrix, m_lightPosition);
        lightTransform = glm::scale(lightTransform, glm::vec3(0.2F));
        m_programLight.setModelMatrix(lightTransform);
        m_programLight.setViewMatrix(unityMatrix);
        m_programLight.setProjectionMatrix(unityMatrix);
    }

    void render() override
    {
        if(!m_camera) return;

        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_vaoCube.use();

        const auto viewMatrix = m_camera->getViewMatrix();
        const auto normalMatrix = glm::mat3 (
                    glm::transpose( glm::inverse(viewMatrix * m_objectModelMatrix) )
                    );
        const auto projMatrix = m_camera->getProjectionMatrix();

        const auto time = static_cast<float>(glfwGetTime());
        const glm::vec3 lightColor { sin(time * 2.0F)
                                   , sin(time * 0.7F)
                                   , sin(time * 1.3F)
                                   };
        const auto ambientColor = lightColor * glm::vec3(0.2F);
        const auto diffuseColor = lightColor * glm::vec3(0.5F);

        m_programMain.use();
        m_programMain.setViewMatrix(viewMatrix);
        m_programMain.setProjectionMatrix(projMatrix);
        m_programMain.setNormalMatrix(normalMatrix);
        m_programMain.setUniform("lightColor", lightColor);

        m_programMaterial.use();
        m_programMaterial.setViewMatrix(viewMatrix);
        m_programMaterial.setProjectionMatrix(projMatrix);
        m_programMaterial.setNormalMatrix(normalMatrix);
        m_programMaterial.setUniform("light.ambient", ambientColor);
        m_programMaterial.setUniform("light.diffuse", diffuseColor);

        m_programLight.use();
        m_programLight.setViewMatrix(m_camera->getViewMatrix());
        m_programLight.setProjectionMatrix(m_camera->getProjectionMatrix());


        m_programMain.use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        m_programMaterial.use();
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
    glm::mat4 m_objectModelMatrix {1.0F};

    Program m_programMain;
    Program m_programLight;
    Program m_programMaterial;

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
    camera->setPosition({0.0F, 1.0F, 10.0F});
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
