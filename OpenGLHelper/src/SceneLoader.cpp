#include "SceneLoader.h"

#include "Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RL {

namespace GL {

Scene SceneLoader::load(const std::string &path)
{
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    const bool isFailed = !scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode;
    if(isFailed)
    {
        logError(__FUNCTION__, path + ": " + importer.GetErrorString());
        return Scene{};
    }

    m_dir = path.substr(0, path.find_last_of("/"));
    processNode(scene->mRootNode, scene);

    return {Scene{m_meshes}};
}

void SceneLoader::processNode(const aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        const auto* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

MeshData SceneLoader::processMesh(const aiMesh *mesh, const aiScene *scene)
{
    MeshData result;

    const auto texCoords0 = mesh->mTextureCoords[0];
    for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        const glm::vec2 texCoords = (texCoords0) ? glm::vec2{texCoords0[i].x, texCoords0[i].y} : glm::vec2{0.0F, 0.0F};
        const auto p = mesh->mVertices[i];
        const auto n = mesh->mNormals[i];
        result.vertices.push_back({ {p.x, p.y, p.z}
                                  , {n.x, n.y, n.z}
                                  , texCoords
                                  });
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const auto face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            result.indices.push_back(face.mIndices[j]);
        }
    }

//    if(mesh->mMaterialIndex >= 0)
//    {
//        const auto* material = scene->mMaterials[mesh->mMaterialIndex];
//    }
    return result;
}

std::vector<Texture> SceneLoader::loadTexture(const aiMaterial *material, TextureType type)
{
    const auto aiTextureType = (type == TextureType::Diffuse) ? aiTextureType_DIFFUSE : aiTextureType_SPECULAR;
    const auto texturesCount = material->GetTextureCount(aiTextureType);

    std::vector<Texture> textures;
    textures.reserve(texturesCount);
    for(unsigned int i = 0; i < texturesCount; ++i)
    {
        aiString textureName;
        material->GetTexture(aiTextureType, i, &textureName);

//        textures.push_back({ 0
//                           , type
//                           , textureName.C_Str()
//                           });
    }

    return textures;
}

} // namespace GL

} // namespace RL
