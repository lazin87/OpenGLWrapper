#include "Program.h"

#include "Logger.h"

#include <glad/glad.h>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>

namespace RL {

namespace GL {

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

ShaderType getShaderType(const std::string &fileName)
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
    const auto extensionBeforeStart = fileName.rfind(".");
    if(extensionBeforeStart == std::string::npos)
        return ShaderType::INVALID;

    std::string extension = fileName.substr(extensionBeforeStart + 1U);
    std::transform(std::cbegin(extension), std::cend(extension), std::begin(extension),
                   [](const char c) { return std::tolower(c); } );

    auto shaderTypeItr = stringToShaderType.find(extension);
    if(shaderTypeItr == std::cend(stringToShaderType))
    {
        logError(__FUNCTION__, "Can not determine the shader type or it is not supported by OpenGL: " + fileName);
        return ShaderType::INVALID;
    }
    else
    {
        return shaderTypeItr->second;
    }
}

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

void Program::build()
{
    loadShaders();

    m_glId = glCreateProgram();
    for(const auto& s: m_shaders)
    {
        if(s.glId == 0) continue;
        glAttachShader(m_glId, s.glId);
    }
    glLinkProgram(m_glId);

    for(auto& s: m_shaders)
    {
        if(s.glId == 0) continue;
        glDeleteShader(s.glId);
        s.glId = 0;
    }
}

unsigned int Program::getId() const noexcept
{
    return m_glId;
}

void Program::loadShaders()
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
        }
        else
        {
            logError(__FUNCTION__, "Can not find file: " + s.path);
        }
    }
}

} // namespace GL

} // namespace RL
