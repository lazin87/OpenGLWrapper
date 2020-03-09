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
        unsigned int diffuseNbr = 1;
        unsigned int specularNbr = 1;
        const auto texturesCnt = static_cast<int>(m.textures.size());
        for(int i = 0; i < texturesCnt; ++i)
        {
            const std::string textureName = (m.textures[i].getType() == TextureType::Specular) ?
                        "material.specular" + std::to_string(specularNbr++)
                      : "material.diffuse" + std::to_string(diffuseNbr++);
            m.textures[i].use(i);
            p.setUniform(textureName, i);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(m.vao);
        glDrawElements(GL_TRIANGLES, m.indicesCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
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

void swap(Scene &first, Scene &second) noexcept
{
    std::swap(first.m_meshes, second.m_meshes);
}

} // namespace GL

} // namespace RL
