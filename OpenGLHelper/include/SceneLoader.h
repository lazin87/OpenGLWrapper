#pragma once

#include "Scene.h"

#include <string>
#include <vector>

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

namespace RL {

namespace GL {

class SceneLoader
{
public:
    SceneLoader() = default;
    ~SceneLoader() = default;

    SceneLoader(const SceneLoader&) = delete;
    SceneLoader& operator=(const SceneLoader&) = delete;
    SceneLoader(SceneLoader&&) = delete;
    SceneLoader& operator=(SceneLoader&&) = delete;

    Scene load(const std::string& path);

private:
    void processNode(const aiNode *node, const aiScene* scene);
    MeshData processMesh(const aiMesh *mesh, const aiScene* scene);
    std::vector<Texture> loadTexture(const aiMaterial* material, TextureType type);

    std::vector<MeshData> m_meshes;

    std::string m_dir {""};
};

} // namespace GL

} // namespace RL
