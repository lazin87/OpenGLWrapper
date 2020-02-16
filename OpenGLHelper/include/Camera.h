#pragma once

#include <glm/glm.hpp>

namespace RL {

namespace GL {

struct CameraParams
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

struct CameraOpticParams
{
    float fovDegrees;
    float aspectRatio;
    float near;
    float far;
};

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    void setPosition(const glm::vec3& p);
    void setFront(const glm::vec3& f);
    void setUpVector(const glm::vec3& u);
    CameraParams getParams() const;

    void setFov(const float degrees);
    void setAspectRatio(const float a);
    void setNearField(const float n);
    void setFarField(const float f);
    CameraOpticParams getOpticParams() const;

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

private:
    CameraParams m_params{{0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}};
    CameraOpticParams m_opticParams {45.0F, 1.0F, 0.1F, 100.0F};

    mutable bool m_viewDirtyFlag {true};
    mutable glm::mat4 m_viewMatrix {1.0F};
    mutable bool m_projectionDirtyFlag {true};
    mutable glm::mat4 m_projectionMatrix {1.0F};
};

} // namespace GL

} // namespace RL
