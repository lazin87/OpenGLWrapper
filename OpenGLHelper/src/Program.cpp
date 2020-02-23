#include "Program.h"

#include "Logger.h"
#include "Utilities.h"

#include <glad/glad.h>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>
#include <optional>

namespace RL {

namespace GL {

namespace {

int toGLShaderType(const ShaderType type)
{
    switch(type)
    {
    case ShaderType::VERTEX_SHADER:
        return GL_VERTEX_SHADER;
    case ShaderType::GEOMETRY_SHADER:
        return GL_GEOMETRY_SHADER;
    case ShaderType::FRAGMENT_SHADER:
        return GL_FRAGMENT_SHADER;
#ifdef GL_TESS_CONTROL_SHADER
    case ShaderType::TESSALATION_CONTROL_SHADER:
        return GL_TESS_CONTROL_SHADER;
#endif
#ifdef GL_TESS_EVALUATION_SHADER
    case ShaderType::TESSALATION_EVALUATION_SHADER:
        return GL_TESS_EVALUATION_SHADER;
#endif
#ifdef GL_COMPUTE_SHADER
    case ShaderType::COMPUTE_SHADER:
        return GL_COMPUTE_SHADER;
#endif
    case ShaderType::INVALID:
        return GL_INVALID_ENUM;
    default:
        logError(__FUNCTION__, "ShaderType not supported: " + std::to_string(static_cast<int>(type)));
        return GL_INVALID_ENUM;
    }
}

ShaderType getShaderType(const std::string &path)
{
    static const std::map<std::string, ShaderType> stringToShaderType{
         {"vert", ShaderType::VERTEX_SHADER}
#ifdef GL_TESS_CONTROL_SHADER
        ,{"tesc", ShaderType::TESSALATION_CONTROL_SHADER}
#endif
#ifdef GL_TESS_EVALUATION_SHADER
        ,{"tese", ShaderType::TESSALATION_EVALUATION_SHADER}
#endif
        ,{"geom", ShaderType::GEOMETRY_SHADER}
        ,{"frag", ShaderType::FRAGMENT_SHADER}
#ifdef GL_COMPUTE_SHADER
        ,{"comp", ShaderType::COMPUTE_SHADER}
#endif
    };

    auto shaderTypeItr = stringToShaderType.find(getFileExtension(path));
    if(shaderTypeItr == std::cend(stringToShaderType))
    {
        logError(__FUNCTION__, "Can not determine the shader type or it is not supported by OpenGL: " + path);
        return ShaderType::INVALID;
    }
    else
    {
        return shaderTypeItr->second;
    }
}

struct ProgramGuard
{

    ProgramGuard() = delete;
    ProgramGuard(ProgramGuard&&) = delete;

    ~ProgramGuard() { glUseProgram(0); }
};

static std::optional<ProgramGuard> lockSafe(const GLuint id)
{
    GLint current;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if( (current != 0) && (current != id) )
    {
        logWarning(__FUNCTION__, "Can not lock, because another program is in use.");
        return {};
    }

    glUseProgram(id);
    return std::optional<ProgramGuard>{};
}

} // namespace

void setDefault(Shader &s)
{
    s.type = ShaderType::INVALID;
    s.path = std::string{};
    s.glId = 0U;
}

Program::Program()
{
    for(auto &s: m_shaders)
    {
        setDefault(s);
    }
}

Program::~Program()
{
    glDeleteProgram(m_glId);
}

void Program::addShader(const std::string &path)
{
    const auto type = getShaderType(path);
    if(type == ShaderType::INVALID)
    {
        logError(__FUNCTION__, "Can not determine shader type: " + path);
        return;
    }

    auto& s = m_shaders[static_cast<size_t>(type)];
    s.type = type;
    s.path = path;
}

void Program::linkProgram()
{
    m_glId = glCreateProgram();
    for(const auto& s: m_shaders)
    {
        if(s.glId == 0) continue;
        glAttachShader(m_glId, s.glId);
    }

    glLinkProgram(m_glId);
    if(!getLinkStatusGlWrapper(*this))
    {
        logError(__FUNCTION__, getInfoLog(*this));
    }
}

void Program::cleanupShaders()
{
    for(auto& s: m_shaders)
    {
        if(s.glId == 0) continue;
        glDeleteShader(s.glId);
        s.glId = 0;
    }
}

void Program::initTransformMatrices()
{
    m_modelLocation = glGetUniformLocation(m_glId, "model");
    m_viewLocation = glGetUniformLocation(m_glId, "view");
    m_projectionLocation = glGetUniformLocation(m_glId, "projection");
    m_normalMatrixLocation = glGetUniformLocation(m_glId, "normalMatrix");
}

bool Program::getCompileStatusGlWrapper(const Shader &s) const
{
    int success = 0;
    glGetShaderiv(s.glId, GL_COMPILE_STATUS, &success);
    return success != 0;
}

bool Program::getLinkStatusGlWrapper(const Program &p) const
{
    int success = 0;
    glGetProgramiv(p.m_glId, GL_LINK_STATUS, &success);
    return success != 0;
}

void Program::getInfoLogGlWrapper(const Shader &s, const int maxLen, int& len, char* buffer) const
{
    glGetShaderInfoLog(s.glId, maxLen, &len, buffer);
}

void Program::getInfoLogGlWrapper(const Program &p, const int maxLen, int& len, char* buffer) const
{
    glGetProgramInfoLog(p.m_glId, maxLen, &len, buffer);
}

void Program::build()
{
    buildShaders();
    linkProgram();
    cleanupShaders();

    initTransformMatrices();
}

void Program::use()
{
    glUseProgram(m_glId);
}

void Program::setUniform(const std::string &name, glm::vec3 value)
{
    setUniformImpl(name, value);
}

void Program::setModelMatrix(const glm::mat4 &m)
{
    setUniformImpl(m_modelLocation, m);
}

void Program::setViewMatrix(const glm::mat4 &m)
{
    setUniformImpl(m_viewLocation, m);
}

void Program::setProjectionMatrix(const glm::mat4 &m)
{
    setUniformImpl(m_projectionLocation, m);
}

void Program::setNormalMatrix(const glm::mat3 &m)
{
    setUniformImpl(m_normalMatrixLocation, m);
}

unsigned int Program::getId() const noexcept
{
    return m_glId;
}

void Program::setUniformGlWrapper(int location, const glm::mat3 &m) const
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::setUniformGlWrapper(int location, const glm::mat4 &m) const
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::setUniformGlWrapper(int location, const glm::vec3 &v) const
{
    glUniform3f(location, v[0], v[1], v[2]);
}

void Program::buildShaders()
{
    for(auto& s: m_shaders)
    {
        if(s.type == ShaderType::INVALID) continue;

        std::string shaderSource;
        std::ifstream f(s.path);
        if(f.is_open())
        {
            f.seekg(0, std::ios::end);
            shaderSource.reserve(f.tellg());
            f.seekg(0, std::ios::beg);
            shaderSource.assign((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());
            f.close();

            s.glId = glCreateShader(toGLShaderType(s.type));
            const char* shaderSourceCStr = shaderSource.c_str();
            glShaderSource(s.glId, 1, &shaderSourceCStr, nullptr);
            glCompileShader(s.glId);

            if(!getCompileStatusGlWrapper(s))
            {
                logError(s.path, getInfoLog(s));
            }
        }
        else
        {
            logError(__FUNCTION__, "Can not find file: " + s.path);
        }
    }
}

int Program::getUniformLocation(const std::string &name)
{
    int location = INVALID_LOCATION;

    const auto u = m_uniformsLocation.find(name);
    if(u == m_uniformsLocation.cend())
    {
        location = glGetUniformLocation(m_glId, name.c_str());
        std::string n2 = name;
        m_uniformsLocation.insert({n2, location});
        if(location == INVALID_LOCATION)
            logWarning(__FUNCTION__, "Uniform not found: " + name);
    }
    else
    {
        location = u->second;
    }

    return location;
}

} // namespace GL

} // namespace RL
