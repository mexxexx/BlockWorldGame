#ifndef __BLOCKWORLD_CHUNKCONTAINER_H__
#define __BLOCKWORLD_CHUNKCONTAINER_H__

#include <vector>
#include <memory>

#include "chunk.h"

namespace bwg
{
class ChunkContainer
{
    public:
        ChunkContainer(const int maxSizeX, const int maxSizeY, const int maxSizeZ);
        void generateInitialChunks();
        void getVisibleFaces(std::vector<GLfloat> &vertices, std::vector<int> &indices) const;
    private:
        int maxSizeX, maxSizeY, maxSizeZ;
        std::vector<std::unique_ptr<Chunk>> chunks;
};
} // namespace engine

#endif