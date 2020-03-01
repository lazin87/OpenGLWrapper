#include "Camera.h"
#include "WindowContext.h"
#include "IRenderer.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "FlyCameraControler.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <vector>

using namespace RL::GL;

class Lesson08: public IRenderer
{

public:
    const std::vector<float> cube_vertices = {
        -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F,  0.0F,
         0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F,  0.0F,
         0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F,  1.0F,
         0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  1.0F,  1.0F,
        -0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F,  1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,  0.0F,  0.0F,

        -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   1.0F, 1.0F,
        -0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,   0.0F, 0.0F,

        -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 0.0F,
        -0.5F,  0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 1.0F,
        -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
        -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  0.0F, 0.0F,
        -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,  1.0F, 0.0F,

         0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 0.0F,
         0.5F,  0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  0.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  1.0F, 0.0F,

        -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  1.0F, 0.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 0.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,  0.0F, 1.0F,

        -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 1.0F,
         0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F, 0.0F,
        -0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 0.0F,
        -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,  0.0F, 1.0F
    };

    void init() override
    {
        glEnable(GL_DEPTH_TEST);

        m_diffuseMap.load("resources/container2.png");
        m_specularMap.load("resources/container2_specular.png");

        m_programLight.addShader("resources/vs.vert");
        m_programLight.addShader("resources/fs_light.frag");
        m_programLight.build();

        m_programMixedLight.addShader("resources/vs.vert");
        m_programMixedLight.addShader("resources/fs_light_mixed.frag");
        m_programMixedLight.build();

        m_vaoCube.setVericesData(cube_vertices);
        m_vaoCube.setVertexAttribPointersConfig({ 0
                                                , GLSLType::vec3
                                                , false
                                                , 8 * sizeof(float)
                                                , (void*) 0
                                                });
        m_vaoCube.setVertexAttribPointersConfig({ 1
                                                , GLSLType::vec3
                                                , false
                                                , 8 * sizeof(float)
                                                , (void*) (3 * sizeof(float))
                                                });
        m_vaoCube.setVertexAttribPointersConfig({ 2
                                                , GLSLType::vec2
                                                , false
                                                , 8 * sizeof(float)
                                                , (void*) (6 * sizeof(float))
                                                });

        m_vaoCube.build();

        const auto unityMatrix = glm::mat4{1.0F};
        auto materialObjTransform = glm::translate(unityMatrix, {1.2F, 0.0F, 2.0F});

        m_programMixedLight.use();
        m_programMixedLight.setModelMatrix(materialObjTransform);
        m_programMixedLight.setViewMatrix(unityMatrix);
        m_programMixedLight.setProjectionMatrix(unityMatrix);
        m_programMixedLight.setNormalMatrix(unityMatrix);
        m_programMixedLight.setUniform("material.diffuse", 0);
        m_programMixedLight.setUniform("material.specular", 1);
        m_programMixedLight.setUniform("material.shininess", 32.0F);
        m_programMixedLight.setUniform("dirLight.ambient", glm::vec3(0.1F));
        m_programMixedLight.setUniform("dirLight.diffuse", {0.5F, 0.5F, 0.5F});
        m_programMixedLight.setUniform("dirLight.specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("dirLight.direction", m_sunDirection);
        m_programMixedLight.setUniform("pointLight[0].ambient", glm::vec3(0.1F));
        m_programMixedLight.setUniform("pointLight[0].diffuse", glm::vec3(1.0F));
        m_programMixedLight.setUniform("pointLight[0].specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("pointLight[0].position", m_pointLightPositions[0]);
        m_programMixedLight.setUniform("pointLight[0].Kc", 1.0F);
        m_programMixedLight.setUniform("pointLight[0].Kl", 0.09F);
        m_programMixedLight.setUniform("pointLight[0].Kq", 0.032F);
        m_programMixedLight.setUniform("pointLight[1].ambient", glm::vec3(0.1F));
        m_programMixedLight.setUniform("pointLight[1].diffuse", glm::vec3(1.0F));
        m_programMixedLight.setUniform("pointLight[1].specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("pointLight[1].position", m_pointLightPositions[1]);
        m_programMixedLight.setUniform("pointLight[1].Kc", 1.0F);
        m_programMixedLight.setUniform("pointLight[1].Kl", 0.09F);
        m_programMixedLight.setUniform("pointLight[1].Kq", 0.032F);
        m_programMixedLight.setUniform("pointLight[2].ambient", glm::vec3(0.1F));
        m_programMixedLight.setUniform("pointLight[2].diffuse", glm::vec3(1.0F));
        m_programMixedLight.setUniform("pointLight[2].specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("pointLight[2].position", m_pointLightPositions[2]);
        m_programMixedLight.setUniform("pointLight[2].Kc", 1.0F);
        m_programMixedLight.setUniform("pointLight[2].Kl", 0.09F);
        m_programMixedLight.setUniform("pointLight[2].Kq", 0.032F);
        m_programMixedLight.setUniform("pointLight[3].ambient", glm::vec3(0.1F));
        m_programMixedLight.setUniform("pointLight[3].diffuse", glm::vec3(1.0F));
        m_programMixedLight.setUniform("pointLight[3].specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("pointLight[3].position", m_pointLightPositions[3]);
        m_programMixedLight.setUniform("pointLight[3].Kc", 1.0F);
        m_programMixedLight.setUniform("pointLight[3].Kl", 0.09F);
        m_programMixedLight.setUniform("pointLight[3].Kq", 0.032F);
        m_programMixedLight.setUniform("spotLight.diffuse", glm::vec3(1.0F));
        m_programMixedLight.setUniform("spotLight.specular", {1.0F, 1.0F, 1.0F});
        m_programMixedLight.setUniform("spotLight.position", m_lightPosition);
        m_programMixedLight.setUniform("spotLight.direction", {0.0F, 0.0F, 1.0F});
        m_programMixedLight.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5F)));
        m_programMixedLight.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0F)));

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
        const auto projMatrix = m_camera->getProjectionMatrix();

        m_diffuseMap.use(0);
        m_specularMap.use(1);

        const auto cameraParams = m_camera->getParams();
        m_programMixedLight.use();
        m_programMixedLight.setViewMatrix(viewMatrix);
        m_programMixedLight.setProjectionMatrix(projMatrix);
        m_programMixedLight.setUniform("pointLight[0].position", glm::vec3(viewMatrix * glm::vec4(m_pointLightPositions[0], 1.0F)));
        m_programMixedLight.setUniform("pointLight[1].position", glm::vec3(viewMatrix * glm::vec4(m_pointLightPositions[1], 1.0F)));
        m_programMixedLight.setUniform("pointLight[2].position", glm::vec3(viewMatrix * glm::vec4(m_pointLightPositions[2], 1.0F)));
        m_programMixedLight.setUniform("pointLight[3].position", glm::vec3(viewMatrix * glm::vec4(m_pointLightPositions[3], 1.0F)));
        m_programMixedLight.setUniform("spotLight.position", cameraParams.position);
        m_programMixedLight.setUniform("spotLight.direction", cameraParams.front);

        int counter = 0;
        for(const auto& pos: m_cubesPos)
        {
            glm::mat4 modelMat = glm::mat4(1.0F);
            modelMat = glm::translate(modelMat, pos);
            const float angle = 20.0F * static_cast<float>(counter++);
            modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            const auto normalMatrix = glm::mat3 (
                        glm::transpose( glm::inverse(viewMatrix * modelMat) )
                        );

            m_programMixedLight.setModelMatrix(modelMat);
            m_programMixedLight.setNormalMatrix(normalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        m_programLight.use();
        m_programLight.setViewMatrix(m_camera->getViewMatrix());
        m_programLight.setProjectionMatrix(m_camera->getProjectionMatrix());

        for(const auto position: m_pointLightPositions)
        {
            glm::mat4 modelMat = glm::mat4(1.0F);
            modelMat = glm::translate(modelMat, position);
            modelMat = glm::scale(modelMat, glm::vec3(0.2F));
            m_programLight.setModelMatrix(modelMat);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
    }

    void set(std::shared_ptr<Camera> c)
    {
        m_camera = std::move(c);
    }

private:
    std::array<glm::vec3, 10> m_cubesPos {
          glm::vec3( 0.0f,  0.0f,  0.0f),
          glm::vec3( 2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3( 2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3( 1.3f, -2.0f, -2.5f),
          glm::vec3( 1.5f,  2.0f, -2.5f),
          glm::vec3( 1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::mat4 m_objectModelMatrix {1.0F};

    Program m_programLight;
    Program m_programMixedLight;

    std::shared_ptr<Camera> m_camera;

    VertexArrayObject m_vaoCube;
    Texture2D m_diffuseMap;
    Texture2D m_specularMap;


    glm::vec3 m_lightPosition {1.2F, 1.0F, 2.0F};
    glm::vec3 m_sunDirection {-0.2F, -1.0F, -0.3F};

    std::array<glm::vec3, 4> m_pointLightPositions {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
};

int main()
{
    WindowContext window{ {3, 3, true} };
    window.createWindow( {"Lesson08", 800, 600, 0, 0} );
    window.enableMouseHandling();

    auto camera = std::make_shared<Camera>();
    camera->setPosition({0.0F, 1.0F, 10.0F});
    camera->setFront({0.0F, 0.0F, -1.0F});
    camera->setFov(45.0F);
    camera->setAspectRatio(800.0F / 600.0F);
    camera->setNearField(0.1F);
    camera->setFarField(100.0F);

    auto renderer = std::make_shared<Lesson08>();
    renderer->set(camera);
    window.set(renderer);

    auto inputControler = std::make_shared<FlyCameraControler>();
    inputControler->set(camera);
    window.set(inputControler);

    window.show();

    return 0;
}
