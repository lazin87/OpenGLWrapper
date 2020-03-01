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

        m_programDirectional.addShader("resources/vs.vert");
        m_programDirectional.addShader("resources/fs_light_directional.frag");
        m_programDirectional.build();

        m_programPointlight.addShader("resources/vs.vert");
        m_programPointlight.addShader("resources/fs_light_pointlight.frag");
        m_programPointlight.build();

        m_programSpotlight.addShader("resources/vs.vert");
        m_programSpotlight.addShader("resources/fs_light_spotlight.frag");
        m_programSpotlight.build();

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

        m_programDirectional.use();
        m_programDirectional.setModelMatrix(materialObjTransform);
        m_programDirectional.setViewMatrix(unityMatrix);
        m_programDirectional.setProjectionMatrix(unityMatrix);
        m_programDirectional.setNormalMatrix(unityMatrix);
        m_programDirectional.setUniform("material.shininess", 32.0F);
        m_programDirectional.setUniform("light.diffuse", {0.5F, 0.5F, 0.5F});
        m_programDirectional.setUniform("light.specular", {1.0F, 1.0F, 1.0F});
        m_programDirectional.setUniform("light.direction", m_sunDirection);

        m_programPointlight.use();
        m_programPointlight.setModelMatrix(materialObjTransform);
        m_programPointlight.setViewMatrix(unityMatrix);
        m_programPointlight.setProjectionMatrix(unityMatrix);
        m_programPointlight.setNormalMatrix(unityMatrix);
        m_programPointlight.setUniform("material.diffuse", 0);
        m_programPointlight.setUniform("material.specular", 1);
        m_programPointlight.setUniform("material.shininess", 32.0F);
        m_programPointlight.setUniform("light.ambient", glm::vec3(0.1F));
        m_programPointlight.setUniform("light.diffuse", glm::vec3(1.0F));
        m_programPointlight.setUniform("light.specular", {1.0F, 1.0F, 1.0F});
        m_programPointlight.setUniform("light.position", m_lightPosition);
        m_programPointlight.setUniform("light.Kc", 1.0F);
        m_programPointlight.setUniform("light.Kl", 0.09F);
        m_programPointlight.setUniform("light.Kq", 0.032F);

        m_programSpotlight.use();
        m_programSpotlight.setModelMatrix(materialObjTransform);
        m_programSpotlight.setViewMatrix(unityMatrix);
        m_programSpotlight.setProjectionMatrix(unityMatrix);
        m_programSpotlight.setNormalMatrix(unityMatrix);
        m_programSpotlight.setUniform("material.diffuse", 0);
        m_programSpotlight.setUniform("material.specular", 1);
        m_programSpotlight.setUniform("material.shininess", 32.0F);
        m_programSpotlight.setUniform("light.ambient", glm::vec3(0.1F));
        m_programSpotlight.setUniform("light.diffuse", glm::vec3(1.0F));
        m_programSpotlight.setUniform("light.specular", {1.0F, 1.0F, 1.0F});
        m_programSpotlight.setUniform("light.position", m_lightPosition);
        m_programSpotlight.setUniform("light.direction", {0.0F, 0.0F, 1.0F});
        m_programSpotlight.setUniform("light.cutOff", glm::cos(glm::radians(12.5F)));
        m_programSpotlight.setUniform("light.outerCutOff", glm::cos(glm::radians(15.0F)));

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
        const auto lightPosition =  glm::vec3(viewMatrix * glm::vec4(m_lightPosition, 1.0F));

        m_diffuseMap.use(0);
        m_specularMap.use(1);

//        m_programDirectional.use();
//        m_programDirectional.setViewMatrix(viewMatrix);
//        m_programDirectional.setProjectionMatrix(projMatrix);
//        m_programDirectional.setUniform("material.diffuse", 0);
//        m_programDirectional.setUniform("material.specular", 1);
//        m_programDirectional.setUniform("light.ambient", glm::vec3(0.1F));
//        m_programDirectional.setUniform("light.diffuse", glm::vec3(1.0F));

//        m_programPointlight.use();
//        m_programPointlight.setViewMatrix(viewMatrix);
//        m_programPointlight.setProjectionMatrix(projMatrix);
//        m_programPointlight.setUniform("light.position", lightPosition);


        const auto cameraParams = m_camera->getParams();
        m_programSpotlight.use();
        m_programSpotlight.setViewMatrix(viewMatrix);
        m_programSpotlight.setProjectionMatrix(projMatrix);
        m_programSpotlight.setUniform("light.position", cameraParams.position);
        m_programSpotlight.setUniform("light.direction", cameraParams.front);

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

//            m_programDirectional.setModelMatrix(modelMat);
//            m_programDirectional.setNormalMatrix(normalMatrix);

//            m_programPointlight.setModelMatrix(modelMat);
//            m_programPointlight.setNormalMatrix(normalMatrix);

            m_programSpotlight.setModelMatrix(modelMat);
            m_programSpotlight.setNormalMatrix(normalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        m_programLight.use();
        m_programLight.setViewMatrix(m_camera->getViewMatrix());
        m_programLight.setProjectionMatrix(m_camera->getProjectionMatrix());

        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    Program m_programDirectional;
    Program m_programPointlight;
    Program m_programSpotlight;

    std::shared_ptr<Camera> m_camera;

    VertexArrayObject m_vaoCube;
    Texture2D m_diffuseMap;
    Texture2D m_specularMap;


    glm::vec3 m_lightPosition {1.2F, 1.0F, 2.0F};
    glm::vec3 m_sunDirection {-0.2F, -1.0F, -0.3F};
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
