#ifndef __BLOCKWORLDGAME_TERRAIN_H__
#define __BLOCKWORLDGAME_TERRAIN_H__

#include "engine.h"

#include "chunkContainer.h"

namespace bwg
{
class Terrain
{
public:
    void loadContent();
    void unloadContent();
    void generateTerrain();
    void drawTerrain(const engine::Camera &camera) const;

private:
    GLuint terrainVao = 0;
    int numVertices;
    engine::Shader terrainShader;
    engine::Texture terrainTexture;
    std::unique_ptr<ChunkContainer> chunkContainer;
};
} // namespace bwg

#endif