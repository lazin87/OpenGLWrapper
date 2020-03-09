#pragma once

#include "Texture2D.h"

#include <glm/glm.hpp>

#include <vector>

namespace RL {

namespace GL {

class Program;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

using IndexT = unsigned int;
struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<IndexT> indices;
    std::vector<Texture2D> textures;
};

struct Mesh
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indicesCount;
    std::vector<Texture2D> textures;
};

class Scene
{
public:
    Scene() = default;
    ~Scene();

    Scene(std::vector<MeshData> &meshes);

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&& other);
    Scene& operator=(Scene&& other);

    void draw(Program& p);

    friend void swap(Scene& first, Scene& second) noexcept;

private:
    static std::string getUniformPrefix(const TextureType t);

    void addMesh(MeshData &meshData);
    void bindTextures(const std::vector<Texture2D> &textures, Program &p) const;

    std::vector<Mesh> m_meshes;

};

void swap(Scene& first, Scene& second) noexcept;

} // namespace GL

} // namespace RL
