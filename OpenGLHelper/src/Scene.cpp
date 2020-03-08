#include "Scene.h"

#include "Utilities.h"

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

Scene::Scene(const std::vector<MeshData>& meshes)
{
    for(const auto& m: meshes)
    {
        addMesh(m);
    }
}

void Scene::draw()
{
    for(const auto& m: m_meshes)
    {
        glBindVertexArray(m.vao);
        glDrawElements(GL_TRIANGLES, m.indicesCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void Scene::addMesh(const MeshData &meshData)
{
    Mesh m;
    m.indicesCount = meshData.indices.size();
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

    m_meshes.push_back(m);
}

void swap(Scene &first, Scene &second) noexcept
{
    std::swap(first.m_meshes, second.m_meshes);
}

} // namespace GL

} // namespace RL
