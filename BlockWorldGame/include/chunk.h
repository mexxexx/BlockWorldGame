#ifndef __BLOCKWORLDGAME_CHUNK_H__
#define __BLOCKWORLDGAME_CHUNK_H__

#include <vector>

#include "glad/glad.h"

#include "blockType.h"

namespace bwg
{
class Chunk
{
public:
    Chunk(const int chunkOffsetX, const int chunkOffsetY, const int chunkOffsetZ);
    // Chunk dimension in x, y, z coordinate direction
    static const int CHUNK_DIMENSION = 16;
    void generateChunk();
    void getVisibleFaces(std::vector<GLfloat> &vertices, std::vector<int> &indices, int &vertexOffset) const;
    static void loadBlockTypes();

private:
    int chunkOffsetX, chunkOffsetY, chunkOffsetZ;
    BlockType *getBlockType(const int x, const int y, const int z) const;
    bool blockIsSeeThrough(const BlockType *block) const;
    unsigned char blocks[CHUNK_DIMENSION * CHUNK_DIMENSION * CHUNK_DIMENSION];
    static BlockType blockTypes[BlockType::NUMBER_OF_BLOCK_TYPES];
    static void addBlockFrontFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockBackFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockBottomFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockTopFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockLeftFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
    static void addBlockRightFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<int> &indices, const float x, const float y, const float z, const BlockType &blockType);
};
} // namespace bwg

#endif