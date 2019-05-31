#ifndef __BLOCKWORLDGAME_CHUNK_H__
#define __BLOCKWORLDGAME_CHUNK_H__

#include <vector>

#include "glad/glad.h"

#include "blockType.h"
#include "valueNoise.h"

namespace bwg
{
class ChunkContainer;

class Chunk
{
public:
    struct BlockVertexAttribute
    {
        GLfloat position[3];
        GLfloat texCoord[2];
        GLfloat faceIndex;
    };

    Chunk(const ChunkContainer *parentContainer, const int chunkOffsetX, const int chunkOffsetY, const int chunkOffsetZ);
    // Chunk dimension in x, y, z coordinate direction
    static const int CHUNK_DIMENSION = 16;
    void generateChunk(const ValueNoise &noise);
    /*
        Collect vertices and indices of all faces, that are not coverd up
        @param vertexOffset: number of vertices that are in the list, used as offset for the index array
    */
    void getVisibleFaces(std::vector<BlockVertexAttribute> &vertices, std::vector<int> &indices, int &vertexOffset) const;
    static void loadBlockTypes();
    /*
        Returns the BlockType at position x,y,z (nullptr if out of bounds)
    */
    BlockType *getBlockType(const int x, const int y, const int z) const;

private:
    int chunkOffsetX, chunkOffsetY, chunkOffsetZ;
    unsigned char blocks[CHUNK_DIMENSION * CHUNK_DIMENSION * CHUNK_DIMENSION];
    const ChunkContainer *parentContainer;
    static bool blockIsSeeThrough(const BlockType *block);
    static BlockType blockTypes[BlockType::NUMBER_OF_BLOCK_TYPES];
    static const float numTexturesInverse;
    static void addBlockFrontFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockBackFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockBottomFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockTopFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockLeftFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockRightFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
};
} // namespace bwg

#endif