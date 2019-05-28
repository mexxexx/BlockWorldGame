#include "chunkContainer.h"

namespace bwg
{
ChunkContainer::ChunkContainer(const int maxSizeX, const int maxSizeY, const int maxSizeZ) :
    maxSizeX(maxSizeX), maxSizeY(maxSizeY), maxSizeZ(maxSizeZ)
{
    this->chunks = std::vector<std::unique_ptr<Chunk>>(maxSizeX * maxSizeY * maxSizeZ);
}

void 
} // namespace bwg