#include "Scene.h"

#include "Utilities.h"
#include "Program.h"

#include <glad/glad.h>

namespace RL {

namespace GL {

Scene::Scene(Scene &&other)
{
    *this = std::move(other);
}

Scene &Scene::operator=(Scene &&other)
{
    swap(*this, other);
    return *this;
}

Scene::~Scene()
{
    for(const auto& m: m_meshes)
    {
        glDeleteVertexArrays(1, &m.vao);
        glDeleteBuffers(1, &m.vbo);
        glDeleteBuffers(1, &m.ebo);
    }
}

Scene::Scene(std::vector<MeshData>& meshes)
{
    for(auto& m: meshes)
    {
        addMesh(m);
    }
}

void Scene::draw(Program& p)
{
    p.use();

    for(const auto& m: m_meshes)
    {
        bindTextures(m.textures, p);
        glBindVertexArray(m.vao);

        glDrawElements(GL_TRIANGLES, m.indicesCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

std::string Scene::getUniformPrefix(const TextureType t)
{
    switch(t)
    {
    case TextureType::Diffuse:
        return "material.diffuse";
    case TextureType::Specular:
        return "material.specular";
    default:
        return "material.texture";
    }
}

void Scene::addMesh(MeshData &meshData)
{
    Mesh m;
    m.indicesCount = meshData.indices.size();
    m.textures = std::move(meshData.textures);

    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);

    glBufferData( GL_ARRAY_BUFFER
                , getDataSize(meshData.vertices)
                , meshData.vertices.data()
                , GL_STATIC_DRAW
                );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER
                , getDataSize(meshData.indices)
                , meshData.indices.data()
                , GL_STATIC_DRAW
                );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 0
                         , 3
                         , GL_FLOAT
                         , GL_FALSE
                         , sizeof(Vertex)
                         , (void*)0
                         );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer( 1
                         , 3
                         , GL_FLOAT
                         , GL_FALSE
                         , sizeof(Vertex)
                         , (void*)offsetof(Vertex, normal)
                         );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer( 2
                         , 2
                         , GL_FLOAT
                         , GL_FALSE
                         , sizeof(Vertex)
                         , (void*)offsetof(Vertex, texCoords)
                         );

    glBindVertexArray(0);

    m_meshes.emplace_back(std::move(m));
}

void Scene::bindTextures(const std::vector<Texture2D>& textures, Program& p) const
{
    const auto texturesCnt = static_cast<int>(textures.size());
    for( int i = 0, diffuseNbr = 1, specularNbr = 1
       ; i < texturesCnt
       ; ++i)
    {
        const auto& t = textures[i];
        const auto type = t.getType();

        auto name = getUniformPrefix(type);
        name += (type == TextureType::Specular) ? std::to_string(specularNbr++)
              : (type == TextureType::Diffuse)  ? std::to_string(diffuseNbr++)
              : "";

        t.use(i);
        p.setUniform(name, i);
    }
    glActiveTexture(GL_TEXTURE0);
}

void swap(Scene &first, Scene &second) noexcept
{
    std::swap(first.m_meshes, second.m_meshes);
}

} // namespace GL

} // namespace RL
