#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace RL {

namespace GL {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

enum class TextureType: int
{
    Diffuse,
    Specular
};

struct Texture
{
    unsigned int id;
    TextureType type;
//    std::string path;
};

using IndexT = unsigned int;
struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<IndexT> indices;
    std::vector<Texture> textures;
};

struct Mesh
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indicesCount;
};

class Scene
{
public:
    Scene() = default;
    ~Scene();

    Scene(const std::vector<MeshData>& meshes);

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&& other);
    Scene& operator=(Scene&& other);

    void draw();

    friend void swap(Scene& first, Scene& second) noexcept;

private:
    void addMesh(const MeshData& meshData);

    std::vector<Mesh> m_meshes;

};

void swap(Scene& first, Scene& second) noexcept;

} // namespace GL

} // namespace RL
