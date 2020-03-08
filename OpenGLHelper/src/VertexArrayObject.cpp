#include "VertexArrayObject.h"

#include "Utilities.h"

#include <glad/glad.h>

#include <utility>

namespace RL {

namespace GL {

namespace Detail {

size_t getElementsCount(const GLSLType t)
{
    switch(t) {
    case GLSLType::vec2:
        return 2;
    case GLSLType::vec3:
        return 3;
    case GLSLType::vec4:
        return 4;
    default:
        return 0;
    }
}

GLenum getElementType(const GLSLType t)
{
    switch(t) {
    case GLSLType::vec2:
    case GLSLType::vec3:
    case GLSLType::vec4:
        return GL_FLOAT;
    default:
        return GL_INVALID_ENUM;
    }
}

} // namespace Detail

VertexArrayObject::VertexArrayObject(VertexArrayObject &&other)
{
    *this = std::move(other);
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject &&other)
{
    swap(*this, other);
    return *this;
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_vaoGlId);
    glDeleteBuffers(1, &m_vboGlId);
    glDeleteBuffers(1, &m_eboGlId);
}

void VertexArrayObject::setVericesData(const std::vector<float> &data)
{
    m_verticesData = data;
}

void VertexArrayObject::setIndices(const std::vector<unsigned int> &indices)
{
    m_indices = indices;
}

void VertexArrayObject::setVertexAttribPointersConfig(const VertexAttribPointerConfig &data)
{
    m_vertexAttribPointersConfig.push_back(data);
}

void VertexArrayObject::build()
{
    if(m_verticesData.empty()) return;

    glGenBuffers(1, &m_vboGlId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboGlId);
    glBufferData(GL_ARRAY_BUFFER, getDataSize(m_verticesData), m_verticesData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(!m_indices.empty())
    {
        glGenBuffers(1, &m_eboGlId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboGlId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getDataSize(m_indices), m_indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glGenVertexArrays(1, &m_vaoGlId);
    glBindVertexArray(m_vaoGlId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboGlId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboGlId);
    for(const auto& config : m_vertexAttribPointersConfig)
    {
        glVertexAttribPointer( config.index
                             , Detail::getElementsCount(config.type)
                             , Detail::getElementType(config.type)
                             , (config.hasToNormalize ? GL_TRUE : GL_FALSE)
                             , config.stride
                             , config.offset);
        glEnableVertexAttribArray(config.index);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArrayObject::use()
{
    glBindVertexArray(m_vaoGlId);
}

unsigned int VertexArrayObject::getId() const noexcept
{
    return m_vaoGlId;
}

size_t VertexArrayObject::getIndicesCount() const
{
    return m_indices.size();
}

void swap(VertexArrayObject &first, VertexArrayObject &second) noexcept
{
    using std::swap;

    swap(first.m_vaoGlId, second.m_vaoGlId);
    swap(first.m_vboGlId, second.m_vboGlId);
    swap(first.m_eboGlId, second.m_eboGlId);
    swap(first.m_verticesData, second.m_verticesData);
    swap(first.m_indices, second.m_indices);
    swap(first.m_vertexAttribPointersConfig, second.m_vertexAttribPointersConfig);
}

void swap(VertexAttribPointerConfig &first, VertexAttribPointerConfig &second) noexcept
{
    using std::swap;

    swap(first.index, second.index);
    swap(first.type, second.type);
    swap(first.hasToNormalize, second.hasToNormalize);
    swap(first.stride, second.stride);
    swap(first.offset, second.offset);
}

} // namespace GL

} // namespace RL
