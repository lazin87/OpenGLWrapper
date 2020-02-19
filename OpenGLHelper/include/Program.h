#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RL {

namespace GL {

enum class ShaderType: int
{
    INVALID = -1,
    VERTEX_SHADER = 0,
    TESSALATION_CONTROL_SHADER = 1,
    TESSALATION_EVALUATION_SHADER = 2,
    GEOMETRY_SHADER = 3,
    FRAGMENT_SHADER = 4,
    COMPUTE_SHADER = 5,

    COUNT = 6
};

struct Shader
{
    ShaderType type;
    std::string path;
    unsigned int glId;
};

void setDefault(Shader &s);

class Program
{
public:
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(Program&&) = delete;

    Program();
    ~Program();

    void addShader(const std::string& path);
    void build();
    void use();

    void setUniform(const std::string& name, glm::vec3 value);

    void setModelMatrix(const glm::mat4& m);
    void setViewMatrix(const glm::mat4& m);
    void setProjectionMatrix(const glm::mat4& m);

    unsigned int getId() const noexcept;

private:
    void loadShaders();

    int getUniformLocation(const std::string& name);

    void setUniformGlWrapper(int location, const glm::mat4& m) const;
    void setUniformGlWrapper(int location, const glm::vec3& v) const;

    template<typename T>
    void setUniformImpl(const std::string& name, const T& value);
    template<typename T>
    void setUniformImpl(const int location, const T& value);

    unsigned int m_glId {0};

    const int INVALID_LOCATION {-1};
    int m_modelLocation {INVALID_LOCATION};
    int m_viewLocation {INVALID_LOCATION};
    int m_projectionLocation {INVALID_LOCATION};

    std::array<Shader, static_cast<size_t>(ShaderType::COUNT)> m_shaders;
    std::unordered_map<std::string, int> m_uniformsLocation{};
};

template<typename T>
void Program::setUniformImpl(const std::string &name, const T& value)
{
    const auto l = getUniformLocation(name);
    setUniformImpl(l, value);
}

template<typename T>
void Program::setUniformImpl(const int location, const T& value)
{
    if(location == INVALID_LOCATION) return;
    setUniformGlWrapper(location, value);
}

} // namespace GL

} // namespace RL
