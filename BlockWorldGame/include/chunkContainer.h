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
    /* Initializes a ChunkContainer 
        @param maxSizeX: number of chunks that are held in the x direction
        @param maxSizeY: number of chunks that are held in the y direction
        @param maxSizeZ: number of chunks that are held in the z direction
    */
    ChunkContainer(const int maxSizeX, const int maxSizeY, const int maxSizeZ, const ValueNoise &noise);
    void generateInitialChunks();
    void getVisibleFaces(std::vector<Chunk::BlockVertexAttribute> &vertices, std::vector<int> &indices) const;

    /*
        Returns the BlockType at position x,y,z (nullptr if out of bounds) in world coordinates
    */
    BlockType *getBlockType(const int x, const int y, const int z) const;

private:
    const int maxSizeX, maxSizeY, maxSizeZ;
    int minChunkOffsetX, minChunkOffsetY, minChunkOffsetZ;
    std::vector<std::unique_ptr<Chunk>> chunks;
    const ValueNoise &noise;
    /* Converts three coordiantes to one for 1D array use */
    int getIndex(const int x, const int y, const int z) const;
    /* Converts world x,y,z coordinates into coordinates of a chunk and its blocks */
    void worldPositionToLocalSpace(const int xWorldPos, const int yWorldPos, const int zWorldPos, int &xBlock, int &yBlock, int &zBlock, int &xChunk, int &yChunk, int &zChunk) const;
    /* Checks if there exists a chunk at the give x,y,z offset*/
    bool chunkIsLoaded(const int chunkX, const int chunkY, const int chunkZ) const;
};
} // namespace bwg

#endif