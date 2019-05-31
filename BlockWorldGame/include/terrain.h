#ifndef __BLOCKWORLDGAME_TERRAIN_H__
#define __BLOCKWORLDGAME_TERRAIN_H__

#include <memory>

#include "engine.h"

#include <glm/glm.hpp>

#include "chunkContainer.h"
#include "valueNoise.h"

namespace bwg
{
class Terrain
{
public:
    static const int VISIBLE_CHUNKS_X = 17; 
    static const int VISIBLE_CHUNKS_Z = 17;
    static const int TERRAIN_HEIGHT_CHUNKS = 4;
    void loadContent();
    void unloadContent();
    void generateTerrain(const int seed);
    void drawTerrain(const engine::Camera &camera) const;

private:
    void createDiffuseMap(); 
    ValueNoise noise;
    GLuint terrainVao = 0;
    int numVertices; 
    glm::vec3 lighColor = glm::vec3(1.0f);
    glm::vec3 lighDirection = glm::normalize(glm::vec3(4.0f, 0.75f, 1.0f));
    engine::Texture1D ambientDiffuseMap;
    float ambientStrength = 0.7f;
    engine::Shader terrainShader;
    engine::Texture2D terrainTexture;
    std::unique_ptr<ChunkContainer> chunkContainer;
};
} // namespace bwg

#endif