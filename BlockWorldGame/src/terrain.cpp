#include "terrain.h"

#include "glad/glad.h"

namespace bwg
{
void Terrain::loadContent()
{
    terrainShader.loadShader("content/shader/terrain.vs", "", "content/shader/terrain.fs");

    terrainTexture.loadTexture("content/textures/terrain.png", GL_RGBA, false);
    terrainTexture.use();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Terrain::unloadContent()
{
    terrainShader.deleteShader();
    terrainTexture.deleteTexture();
    if (terrainVao != 0)
        glDeleteBuffers(1, &terrainVao);
}

void Terrain::generateTerrain()
{
    Chunk::loadBlockTypes();
    chunkContainer = std::unique_ptr<ChunkContainer>(new ChunkContainer(2, 2, 2));
    chunkContainer->generateInitialChunks();

    std::vector<GLfloat> vertices;
    std::vector<GLint> indices;
    chunkContainer->getVisibleFaces(vertices, indices);
    numVertices = indices.size();

    GLuint terrainVbo, terrainEbo;
    glGenBuffers(1, &terrainVbo);
    glGenVertexArrays(1, &terrainVao);
    glGenBuffers(1, &terrainEbo);
    glBindVertexArray(terrainVao);

    glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 5 * sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Terrain::drawTerrain(const engine::Camera &camera) const
{
    terrainTexture.use();
    terrainShader.use();
    terrainShader.setMatrix4fv("viewProjection", camera.getViewProjectionMatrix());
    glBindVertexArray(terrainVao);
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
} // namespace bwg