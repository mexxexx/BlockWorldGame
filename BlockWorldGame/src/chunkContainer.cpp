#include "chunkContainer.h"

#include <memory>
#include <chrono>

#include "engine.h"

namespace bwg
{
ChunkContainer::ChunkContainer(const int maxSizeX, const int maxSizeY, const int maxSizeZ, const ValueNoise &noise) : maxSizeX(maxSizeX), maxSizeY(maxSizeY), maxSizeZ(maxSizeZ), noise(noise)
{
}

void ChunkContainer::generateInitialChunks()
{
    minChunkOffsetX = maxSizeX / 2 - maxSizeX + 1;
    minChunkOffsetY = 0;
    minChunkOffsetZ = maxSizeZ / 2 - maxSizeZ + 1;
    this->chunks = std::vector<std::unique_ptr<Chunk>>(maxSizeX * maxSizeY * maxSizeZ);
    for (int x = 0; x < maxSizeX; x++)
    {
        for (int y = 0; y < maxSizeY; y++)
        {
            for (int z = 0; z < maxSizeZ; z++)
            {
                std::unique_ptr<Chunk> chunk(new Chunk(this, minChunkOffsetX + x, minChunkOffsetY + y, minChunkOffsetZ + z));
                chunk->generateChunk(noise);

                chunks[getIndex(x, y, z)] = std::move(chunk);
            }
        }
    }
}

void ChunkContainer::getVisibleFaces(std::vector<Chunk::BlockVertexAttribute> &vertices, std::vector<int> &indices) const
{

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    int vertexOffset = 0;
    for (int x = 0; x < maxSizeX; x++)
    {
        for (int y = 0; y < maxSizeY; y++)
        {
            for (int z = 0; z < maxSizeZ; z++)
            {
                chunks[getIndex(x, y, z)]->getVisibleFaces(vertices, indices, vertexOffset);
            }
        }
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    engine::Log::d(std::to_string(duration));
}

BlockType *ChunkContainer::getBlockType(const int x, const int y, const int z) const
{
    int xBlock, yBlock, zBlock;
    int xChunk, yChunk, zChunk;
    worldPositionToLocalSpace(x, y, z, xBlock, yBlock, zBlock, xChunk, yChunk, zChunk);
    if (chunkIsLoaded(xChunk, yChunk, zChunk))
        return chunks[getIndex(xChunk - minChunkOffsetX, yChunk - minChunkOffsetY, zChunk - minChunkOffsetZ)]->getBlockType(xBlock, yBlock, zBlock);
    else
        return nullptr;
}

int ChunkContainer::getIndex(const int x, const int y, const int z) const
{
    return x + y * maxSizeX + z * maxSizeX * maxSizeY;
}

void ChunkContainer::worldPositionToLocalSpace(const int xWorldPos, const int yWorldPos, const int zWorldPos, int &xBlock, int &yBlock, int &zBlock, int &xChunk, int &yChunk, int &zChunk) const
{
    xBlock = xWorldPos % Chunk::CHUNK_DIMENSION;
    if (xBlock < 0)
        xBlock += Chunk::CHUNK_DIMENSION;
    yBlock = yWorldPos % Chunk::CHUNK_DIMENSION;
    if (yBlock < 0)
        yBlock += Chunk::CHUNK_DIMENSION;
    zBlock = zWorldPos % Chunk::CHUNK_DIMENSION;
    if (zBlock < 0)
        zBlock += Chunk::CHUNK_DIMENSION;

    xChunk = xWorldPos / Chunk::CHUNK_DIMENSION;
    if (xWorldPos < 0 && xBlock != 0)
        xChunk--;
    yChunk = yWorldPos / Chunk::CHUNK_DIMENSION;
    if (yWorldPos < 0 && yBlock != 0)
        yChunk--;
    zChunk = zWorldPos / Chunk::CHUNK_DIMENSION;
    if (zWorldPos < 0 && zBlock != 0)
        zChunk--;
}

bool ChunkContainer::chunkIsLoaded(const int chunkX, const int chunkY, const int chunkZ) const
{
    return chunkX >= minChunkOffsetX && chunkX < minChunkOffsetX + maxSizeX &&
           chunkY >= minChunkOffsetY && chunkY < minChunkOffsetY + maxSizeY &&
           chunkZ >= minChunkOffsetZ && chunkZ < minChunkOffsetZ + maxSizeZ;
}
} // namespace bwg