#pragma once

#include <array>
#include <string>

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

    unsigned int getId() const noexcept;

private:
    void loadShaders();

    unsigned int m_glId {0};
    std::array<Shader, static_cast<size_t>(ShaderType::COUNT)> m_shaders;
};

} // namespace GL

} // namespace RL
