#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

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
    std::string path;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
};

class SceneLoader
{
public:
    SceneLoader() = default;
    ~SceneLoader() = default;

    SceneLoader(const SceneLoader&) = delete;
    SceneLoader& operator=(const SceneLoader&) = delete;
    SceneLoader(SceneLoader&&) = delete;
    SceneLoader& operator=(SceneLoader&&) = delete;

    bool load(const std::string& path);
    const std::vector<Mesh>& getMeshes() const;

private:
    void processNode(const aiNode *node, const aiScene* scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene* scene);
    std::vector<Texture> loadTexture(const aiMaterial* material, TextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_dir {""};
};

} // namespace GL

} // namespace RL
