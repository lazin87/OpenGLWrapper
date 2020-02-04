#pragma once

#include <vector>

namespace RL {

namespace GL {

enum class GLSLType: int
{
    INVALID = -1,
    vec2,
    vec3,
    vec4
};

struct VertexAttribPointerConfig
{
    unsigned int index;
    GLSLType type;
    bool hasToNormalize;
    size_t stride;
    void* offset;
};

class VertexArrayObject
{
public:
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&&) = delete;
    VertexArrayObject& operator=(VertexArrayObject&&) = delete;

    VertexArrayObject() = default;
    ~VertexArrayObject();

    void setVericesData(const std::vector<float>& data);
    void setIndices(const std::vector<unsigned int>& indices);
    void setVertexAttribPointersConfig(const VertexAttribPointerConfig &data);

    void build();
    unsigned int getId() const noexcept;

private:
    unsigned int m_vaoGlId{0};
    unsigned int m_vboGlId{0};
    unsigned int m_eboGlId{0};
    std::vector<float> m_verticesData;
    std::vector<unsigned int> m_indices;
    std::vector<VertexAttribPointerConfig> m_vertexAttribPointersConfig;
};

} // namespace GL

} // namespace RL
