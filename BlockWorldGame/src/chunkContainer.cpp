#include "chunkContainer.h"

#include <memory>

namespace bwg
{
ChunkContainer::ChunkContainer(const int maxSizeX, const int maxSizeY, const int maxSizeZ) : maxSizeX(maxSizeX), maxSizeY(maxSizeY), maxSizeZ(maxSizeZ)
{
}

void ChunkContainer::generateInitialChunks()
{
    const int chunkOffsetX = maxSizeX / 2 - maxSizeX + 1;
    const int chunkOffsetZ = maxSizeZ / 2 - maxSizeZ + 1;
    this->chunks = std::vector<std::unique_ptr<Chunk>>(maxSizeX * maxSizeY * maxSizeZ);
    for (int x = 0; x < maxSizeX; x++)
    {
        for (int y = 0; y < maxSizeY; y++)
        {
            for (int z = 0; z < maxSizeZ; z++)
            {
                std::unique_ptr<Chunk> chunk(new Chunk((chunkOffsetX + x), y, (chunkOffsetZ + z)));
                chunk->generateChunk();

                chunks[x + y * maxSizeX + z * maxSizeX * maxSizeY] = std::move(chunk);
            }
        }
    }
}

void ChunkContainer::getVisibleFaces(std::vector<GLfloat> &vertices, std::vector<int> &indices) const
{
    int vertexOffset = 0;
    for (int x = 0; x < maxSizeX; x++)
    {
        for (int y = 0; y < maxSizeY; y++)
        {
            for (int z = 0; z < maxSizeZ; z++)
            {
                chunks[x + y * maxSizeX + z * maxSizeX * maxSizeY]->getVisibleFaces(vertices, indices, vertexOffset);
            }
        }
    }
}
} // namespace bwg