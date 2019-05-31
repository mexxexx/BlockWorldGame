#include "terrain.h"

#include "glad/glad.h"

namespace bwg
{
void Terrain::createDiffuseMap()
{
    glm::vec3 colorRight = (ambientStrength + glm::max(lighDirection.x, 0.0f)) * lighColor;
    glm::vec3 colorLeft = (ambientStrength + glm::max(-lighDirection.x, 0.0f)) * lighColor;
    glm::vec3 colorTop = (ambientStrength + glm::max(lighDirection.y, 0.0f)) * lighColor;
    glm::vec3 colorBottom = (ambientStrength + glm::max(-lighDirection.y, 0.0f)) * lighColor;
    glm::vec3 colorFront = (ambientStrength + glm::max(lighDirection.z, 0.0f)) * lighColor;
    glm::vec3 colorBack = (ambientStrength + glm::max(-lighDirection.z, 0.0f)) * lighColor; 

    float ambientDiffuseValues[] = {
        //1x8 Texture
        colorRight.x, colorRight.y, colorRight.z,
        colorLeft.x, colorLeft.y, colorLeft.z,
        colorTop.x, colorTop.y, colorTop.z,
        colorBottom.x, colorBottom.y, colorBottom.z,
        colorFront.x, colorFront.y, colorFront.z,
        colorBack.x, colorBack.y, colorBack.z,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f};

    ambientDiffuseMap.createTextureFromRawData(8, GL_RGB, GL_FLOAT, false, ambientDiffuseValues);
    ambientDiffuseMap.use(0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Terrain::loadContent()
{
    terrainShader.loadShader("content/shader/terrain.vs", "", "content/shader/terrain.fs");

    terrainTexture.loadTexture("content/textures/terrain.png", GL_RGBA, false);
    terrainTexture.use(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    createDiffuseMap();
}

void Terrain::unloadContent()
{
    terrainShader.deleteShader();
    terrainTexture.deleteTexture();
    ambientDiffuseMap.deleteTexture();
    if (terrainVao != 0)
        glDeleteBuffers(1, &terrainVao);
}

void Terrain::generateTerrain(const int seed)
{
    noise.setupGrid(seed, ValueNoise::DEFAULT_GRID_SIZE, Terrain::TERRAIN_HEIGHT_CHUNKS * Chunk::CHUNK_DIMENSION);

    Chunk::loadBlockTypes();
    chunkContainer = std::unique_ptr<ChunkContainer>(new ChunkContainer(VISIBLE_CHUNKS_X, TERRAIN_HEIGHT_CHUNKS, VISIBLE_CHUNKS_Z, noise));
    chunkContainer->generateInitialChunks();

    std::vector<Chunk::BlockVertexAttribute> vertices;
    std::vector<GLint> indices;
    chunkContainer->getVisibleFaces(vertices, indices);
    numVertices = indices.size();

    GLuint terrainVbo, terrainEbo;
    glGenBuffers(1, &terrainVbo);
    glGenVertexArrays(1, &terrainVao);
    glGenBuffers(1, &terrainEbo);
    glBindVertexArray(terrainVao);

    glBindBuffer(GL_ARRAY_BUFFER, terrainVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Chunk::BlockVertexAttribute), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 6 * sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Chunk::BlockVertexAttribute, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Chunk::BlockVertexAttribute, texCoord)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offsetof(Chunk::BlockVertexAttribute, faceIndex)));
    glEnableVertexAttribArray(2);
}

void Terrain::drawTerrain(const engine::Camera &camera) const
{
    terrainTexture.use(0);
    ambientDiffuseMap.use(1);
    terrainShader.use();
    terrainShader.setMatrix4fv("viewProjection", camera.getViewProjectionMatrix());
    terrainShader.setInt("terrainTexture", 0);
    terrainShader.setInt("ambientDiffuseTexture", 1);
    glBindVertexArray(terrainVao);
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
} // namespace bwg