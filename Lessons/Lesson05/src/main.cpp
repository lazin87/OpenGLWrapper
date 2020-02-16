#include "WindowContext.h"
#include "Program.h"
#include "VertexArrayObject.h"
#include "Texture2D.h"
#include "IRenderer.h"
#include "IInputHandler.h"
#include "Camera.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

using namespace RL::GL;

struct EulerAngles
{
    float pitch;
    float yaw;
    float roll;
};

class Lesson05 : public IRenderer
{
    const std::vector<float> square_vertices = {
        // positions            // colors              // texture coords
         0.5F,  0.5F,  0.0F,    1.0F,  0.0F,  0.0F,    1.0F,  1.0F, // top right
         0.5F, -0.5F,  0.0F,    0.0F,  1.0F,  0.0F,    1.0F,  0.0F, // bottom right
        -0.5F, -0.5F,  0.0F,    0.0F,  0.0F,  1.0F,    0.0F,  0.0F, // bottom left
        -0.5F,  0.5F,  0.0F,    1.0F,  1.0F,  0.0F,    0.0F,  1.0F  // top left
    };
    const std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
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

    const std::array<glm::vec3, 10> cubePositions = {
        glm::vec3( 0.0F,  0.0F,  0.0F),
        glm::vec3( 2.0F,  5.0F, -15.0F),
        glm::vec3(-1.5F, -2.2F, -2.5F),
        glm::vec3(-3.8F, -2.0F, -12.3F),
        glm::vec3( 2.4F, -0.4F, -3.5F),
        glm::vec3(-1.7F,  3.0F, -7.5F),
        glm::vec3( 1.3F, -2.0F, -2.5F),
        glm::vec3( 1.5F,  2.0F, -2.5F),
        glm::vec3( 1.5F,  0.2F, -1.5F),
        glm::vec3(-1.3F,  1.0F, -1.5F)
    };

public:
    void init() override
    {
        glEnable(GL_DEPTH_TEST);

        m_program.addShader("resources/vs.vert");
        m_program.addShader("resources/fs.frag");
        m_program.build();

        m_vaoPlane.setVericesData(square_vertices);
        m_vaoPlane.setIndices(indices);
        m_vaoPlane.setVertexAttribPointersConfig({0, GLSLType::vec3, false, 8 * sizeof(float), (void* )0});
        m_vaoPlane.setVertexAttribPointersConfig({2, GLSLType::vec2, false, 8 * sizeof(float), (void* )(6 * sizeof(float))});
        m_vaoPlane.build();

        m_vaoCube.setVericesData(cube_vertices);
        m_vaoCube.setVertexAttribPointersConfig({0, GLSLType::vec3, false, 5*sizeof(float), (void*) 0});
        m_vaoCube.setVertexAttribPointersConfig({2, GLSLType::vec2, false, 5*sizeof(float), (void*) (3 * sizeof(float))});
        m_vaoCube.build();

        m_texture1.load("resources/container.jpg");
        m_texture2.load("resources/awesomeface.png");

        m_modelMatrixLocation = glGetUniformLocation(m_program.getId(), "modelMatrix");
        m_viewMatrixLocation = glGetUniformLocation(m_program.getId(), "viewMatrix");
        m_projectionMatrixLocation = glGetUniformLocation(m_program.getId(), "projectionMatrix");
    }

    void render() override
    {
        if(m_camera)
        {
            const auto viewMatrix = m_camera->getViewMatrix();
            glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

            const auto projectionMatrix = m_camera->getProjectionMatrix();
            glUniformMatrix4fv(m_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        }

        glClearColor(0.5F, 0.1F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture1.getId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture2.getId());

        glBindVertexArray(m_vaoPlane.getId());
        glUseProgram(m_program.getId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindVertexArray(m_vaoCube.getId());
        glUseProgram(m_program.getId());

        int i = 0;
        for(const auto& p: cubePositions)
        {
            glm::mat4 modelMatrix {1.0F};
            modelMatrix = glm::translate(modelMatrix, p);
            const auto angle = 20.0F * (i++ % 10);
            modelMatrix = glm::rotate(modelMatrix, static_cast<float>(glfwGetTime()) * glm::radians(angle), glm::vec3(1.0F, 0.3F, 0.5F));
            glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glUniform1i(glGetUniformLocation(m_program.getId(), "texture1"), 0);
        glUniform1i(glGetUniformLocation(m_program.getId(), "texture2"), 1);
    }

    void set(std::shared_ptr<Camera> c)
    {
        m_camera = std::move(c);
    }

private:
    Program m_program;
    VertexArrayObject m_vaoPlane;
    VertexArrayObject m_vaoCube;
    Texture2D m_texture1;
    Texture2D m_texture2;
    std::shared_ptr<Camera> m_camera;

    unsigned int m_modelMatrixLocation {0};
    unsigned int m_viewMatrixLocation{0};
    unsigned int m_projectionMatrixLocation{0};
};

class InputHandler: public IInputHandler
{
    struct MouseCursor
    {
        glm::vec2 lastPosition;
        float sensitivity;
    };

public:
    void processInput(GLFWwindow *window) override
    {
        auto camera = m_camera.lock();
        if(!camera) return;

        updateLastFrameTime();
        const auto cameraSpeed = 2.5F * m_deltaTime;
        auto cameraParams = camera->getParams();

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraParams.position += cameraSpeed * cameraParams.front;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraParams.position -= cameraSpeed * cameraParams.front;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraParams.position -= glm::normalize(glm::cross(cameraParams.front, cameraParams.up)) * cameraSpeed;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraParams.position += glm::normalize(glm::cross(cameraParams.front, cameraParams.up)) * cameraSpeed;
        }

        camera->setPosition(cameraParams.position);
    }

    void processMouseCursorMove(const double x, const double y) override
    {
        auto camera = m_camera.lock();
        if(!camera) return;

        static bool isFirstCall = true;
        if(isFirstCall)
        {
            m_cursor.lastPosition.x = x;
            m_cursor.lastPosition.y = y;
            isFirstCall = false;
        }

        glm::vec2 positionDelta{
              (static_cast<float>(x) - m_cursor.lastPosition.x)
            , (-static_cast<float>(y) + m_cursor.lastPosition.y)
        };
        m_cursor.lastPosition.x = x;
        m_cursor.lastPosition.y = y;
        positionDelta *= m_cursor.sensitivity;

        m_lookDirection.yaw += positionDelta.x;
        m_lookDirection.pitch += positionDelta.y;
        m_lookDirection.pitch = std::clamp(m_lookDirection.pitch, -89.0F, 89.0F);

        const glm::vec3 direction {
            cos(glm::radians(m_lookDirection.yaw)) * cos(glm::radians(m_lookDirection.pitch)),
            sin(glm::radians(m_lookDirection.pitch)),
            sin(glm::radians(m_lookDirection.yaw)) * cos(glm::radians(m_lookDirection.pitch))
        };
        camera->setFront(direction);
    }

    void processMouseScroll(double xOffset, double yOffset) override
    {
        auto camera = m_camera.lock();
        if(!camera) return;

        auto fov = camera->getOpticParams().fovDegrees - static_cast<float>(yOffset);
        fov = std::clamp(fov, 1.0F, 45.0F);
        camera->setFov(fov);
    }

    void set(std::shared_ptr<Camera> camera)
    {
        m_camera = camera;
    }

private:
    void updateLastFrameTime()
    {
        const auto time = glfwGetTime();
        if(time != m_lastFrameTime)
        {
            m_deltaTime = time - m_lastFrameTime;
            m_lastFrameTime = time;
        }
    }

    std::weak_ptr<Camera> m_camera;
    float m_deltaTime {0.0F};
    float m_lastFrameTime {0.0F};
    MouseCursor m_cursor {{400.0F, 300.0F}, 0.05F};
    EulerAngles m_lookDirection {0.0F, -90.0F, 0.0F};

};

int main()
{
    WindowContext windowContext({3, 3, true});
    windowContext.createWindow({"Hello OpenGL", 800, 600, 0, 0});

    auto renderer = std::make_shared<Lesson05>();
    auto camera = std::make_shared<Camera>();
    camera->setPosition({0.0F, 0.0F, 3.0F});
    camera->setFront({0.0F, 0.0F, -1.0F});
    camera->setFov(45.0F);
    camera->setAspectRatio(800.0F / 600.0F);
    camera->setNearField(0.1F);
    camera->setFarField(100.0F);
    auto inputHandler = std::make_shared<InputHandler>();
    renderer->set(camera);
    inputHandler->set(camera);
    windowContext.set(renderer);
    windowContext.set(inputHandler);
    windowContext.enableMouseHandling();

    windowContext.show();

    return 0;
}