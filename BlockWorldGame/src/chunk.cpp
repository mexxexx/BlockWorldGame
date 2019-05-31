#include "chunk.h"

#include "engine.h"

#include "chunkContainer.h"

namespace bwg
{
BlockType Chunk::blockTypes[] = {};
const float Chunk::numTexturesInverse = 1.0f / 16.0f;

Chunk::Chunk(const ChunkContainer *parentContainer, const int chunkOffsetX, const int chunkOffsetY, const int chunkOffsetZ) : parentContainer(parentContainer), chunkOffsetX(chunkOffsetX), chunkOffsetY(chunkOffsetY), chunkOffsetZ(chunkOffsetZ)
{
}

void Chunk::loadBlockTypes()
{
    BlockType air;
    air.id = BlockType::BlockTypeID::Air;
    air.visibility = BlockType::BlockTypeVisibility::Nothing;
    Chunk::blockTypes[air.id] = air;
    BlockType grass;
    grass.id = BlockType::BlockTypeID::Grass;
    grass.visibility = BlockType::BlockTypeVisibility::Solid;
    grass.left.texCoordUTopLeft = 3;
    grass.left.texCoordVTopLeft = 0;
    grass.right.texCoordUTopLeft = 3;
    grass.right.texCoordVTopLeft = 0;
    grass.top.texCoordUTopLeft = 0;
    grass.top.texCoordVTopLeft = 0;
    grass.bottom.texCoordUTopLeft = 2;
    grass.bottom.texCoordVTopLeft = 0;
    grass.front.texCoordUTopLeft = 3;
    grass.front.texCoordVTopLeft = 0;
    grass.back.texCoordUTopLeft = 3;
    grass.back.texCoordVTopLeft = 0;
    Chunk::blockTypes[grass.id] = grass;
}

void Chunk::generateChunk(const ValueNoise &noise)
{
    int xWorldPos, yWorldPos, zWorldPos = 0;
    for (int x = 0; x < CHUNK_DIMENSION; x++)
    {
        xWorldPos = x + chunkOffsetX * CHUNK_DIMENSION;
        for (int z = 0; z < CHUNK_DIMENSION; z++)
        {
            zWorldPos = z + chunkOffsetZ * CHUNK_DIMENSION;
            float noiseValue = noise.noise2D(xWorldPos, zWorldPos);
            for (int y = 0; y < CHUNK_DIMENSION; y++)
            {
                yWorldPos = y + chunkOffsetY * CHUNK_DIMENSION;
                blocks[x + y * CHUNK_DIMENSION + z * CHUNK_DIMENSION * CHUNK_DIMENSION] = (yWorldPos < noiseValue + 1) ? 1 : 0;
            }
        }
    }
}

bool Chunk::blockIsSeeThrough(const BlockType *block)
{
    return block && (block->visibility == BlockType::Nothing || block->visibility == BlockType::Transparent);
}

void Chunk::getVisibleFaces(std::vector<BlockVertexAttribute> &vertices, std::vector<GLint> &indices, int &vertexOffset) const
{
    for (int x = 0; x < CHUNK_DIMENSION; x++)
    {
        for (int y = 0; y < CHUNK_DIMENSION; y++)
        {
            for (int z = 0; z < CHUNK_DIMENSION; z++)
            {
                unsigned char type = blocks[x + y * CHUNK_DIMENSION + z * CHUNK_DIMENSION * CHUNK_DIMENSION];
                BlockType blockType = Chunk::blockTypes[type];
                int xWorldPos = x + chunkOffsetX * CHUNK_DIMENSION;
                int yWorldPos = y + chunkOffsetY * CHUNK_DIMENSION;
                int zWorldPos = z + chunkOffsetZ * CHUNK_DIMENSION;
                if (blockType.visibility == BlockType::Solid)
                {
                    BlockType *neighbour = getBlockType(x + 1, y, z);
                    if (blockIsSeeThrough(neighbour))
                        addBlockRightFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos, zWorldPos, blockType);

                    neighbour = getBlockType(x, y + 1, z);
                    if (!neighbour || blockIsSeeThrough(neighbour))
                        addBlockTopFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos, zWorldPos, blockType);

                    if (yWorldPos == 0 && xWorldPos == 48 && zWorldPos == -33)
                    {
                        yWorldPos = 0;
                    }
                    neighbour = getBlockType(x, y, z + 1);
                    if (blockIsSeeThrough(neighbour))
                        addBlockFrontFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos, zWorldPos, blockType);
                }
                else if (blockIsSeeThrough(&blockType))
                {
                    if (yWorldPos == 0 && xWorldPos == 47 && zWorldPos == -63)
                    {
                        yWorldPos = 0;
                    }
                    BlockType *neighbour = getBlockType(x + 1, y, z);
                    if (neighbour && !blockIsSeeThrough(neighbour))
                        addBlockLeftFace(vertices, vertexOffset, indices, xWorldPos + 1, yWorldPos, zWorldPos, *neighbour);

                    neighbour = getBlockType(x, y + 1, z);
                    if (neighbour && !blockIsSeeThrough(neighbour))
                        addBlockBottomFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos + 1, zWorldPos, *neighbour);

                    neighbour = getBlockType(x, y, z + 1);
                    if (neighbour && !blockIsSeeThrough(neighbour))
                        addBlockBackFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos, zWorldPos + 1, *neighbour);
                }
            }
        }
    }
}

BlockType *Chunk::getBlockType(const int x, const int y, const int z) const
{
    if (x >= 0 && x < CHUNK_DIMENSION && y >= 0 && y < CHUNK_DIMENSION && z >= 0 && z < CHUNK_DIMENSION)
    {
        unsigned char type = blocks[x + y * CHUNK_DIMENSION + z * CHUNK_DIMENSION * CHUNK_DIMENSION];
        return &blockTypes[type];
    }
    else
    {
        int xWorldPos = x + chunkOffsetX * CHUNK_DIMENSION;
        int yWorldPos = y + chunkOffsetY * CHUNK_DIMENSION;
        int zWorldPos = z + chunkOffsetZ * CHUNK_DIMENSION;
        return parentContainer->getBlockType(xWorldPos, yWorldPos, zWorldPos);
    }
}

void Chunk::addBlockRightFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 0.5f / 8.0f;
    BlockVertexAttribute rightFaceVertices[]{
        {{x + 1.0f, y, z},
         {(blockType.right.texCoordUTopLeft) * numTexturesInverse, (blockType.right.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y + 1.0f, z},
         {(blockType.right.texCoordUTopLeft) * numTexturesInverse, (blockType.right.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y + 1.0f, z + 1.0f},
         {(blockType.right.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.right.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y, z + 1.0f},
         {(blockType.right.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.right.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex}};

    int rightFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(rightFaceIndices), std::end(rightFaceIndices));
    vertices.insert(vertices.end(), std::begin(rightFaceVertices), std::end(rightFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockLeftFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 1.5f / 8.0f;
    BlockVertexAttribute leftFaceVertices[]{
        {{x, y + 1.0f, z + 1.0f},
         {(blockType.left.texCoordUTopLeft) * numTexturesInverse, (blockType.left.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y + 1.0f, z},
         {(blockType.left.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.left.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y, z},
         {(blockType.left.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.left.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x, y, z + 1.0f},
         {(blockType.left.texCoordUTopLeft) * numTexturesInverse, (blockType.left.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex}};

    int leftFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(leftFaceIndices), std::end(leftFaceIndices));
    vertices.insert(vertices.end(), std::begin(leftFaceVertices), std::end(leftFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockTopFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 2.5f / 8.0f;
    BlockVertexAttribute topFaceVertices[]{
        {{x + 1.0f, y + 1.0f, z + 1.0f},
         {(blockType.top.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.top.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y + 1.0f, z},
         {(blockType.top.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.top.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y + 1.0f, z},
         {(blockType.top.texCoordUTopLeft) * numTexturesInverse, (blockType.top.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y + 1.0f, z + 1.0f},
         {(blockType.top.texCoordUTopLeft) * numTexturesInverse, (blockType.top.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex}};

    int topFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(topFaceIndices), std::end(topFaceIndices));
    vertices.insert(vertices.end(), std::begin(topFaceVertices), std::end(topFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockBottomFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 3.5f / 8.0f;
    BlockVertexAttribute bottomFaceVertices[]{
        {{x, y, z},
         {(blockType.bottom.texCoordUTopLeft) * numTexturesInverse, (blockType.bottom.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y, z},
         {(blockType.bottom.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.bottom.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y, z + 1.0f},
         {(blockType.bottom.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.bottom.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y, z + 1.0f},
         {(blockType.bottom.texCoordUTopLeft) * numTexturesInverse, (blockType.bottom.texCoordVTopLeft) * numTexturesInverse},
         faceIndex}};

    int bottomFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(bottomFaceIndices), std::end(bottomFaceIndices));
    vertices.insert(vertices.end(), std::begin(bottomFaceVertices), std::end(bottomFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockFrontFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 4.5f / 8.0f;
    BlockVertexAttribute frontFaceVertices[] = {
        {{x, y, z + 1.0f},
         {(blockType.front.texCoordUTopLeft) * numTexturesInverse, (blockType.front.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y, z + 1.0f},
         {(blockType.front.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.front.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y + 1.0f, z + 1.0f},
         {(blockType.front.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.front.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x, y + 1.0f, z + 1.0f},
         {(blockType.front.texCoordUTopLeft) * numTexturesInverse, (blockType.front.texCoordVTopLeft) * numTexturesInverse},
         faceIndex}};

    int frontFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(frontFaceIndices), std::end(frontFaceIndices));
    vertices.insert(vertices.end(), std::begin(frontFaceVertices), std::end(frontFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockBackFace(std::vector<BlockVertexAttribute> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    const float faceIndex = 5.5f / 8.0;
    BlockVertexAttribute backFaceVertices[]{
        {{x + 1.0f, y + 1.0f, z},
         {(blockType.back.texCoordUTopLeft) * numTexturesInverse, (blockType.back.texCoordVTopLeft) * numTexturesInverse},
         faceIndex},
        {{x + 1.0f, y, z},
         {(blockType.back.texCoordUTopLeft) * numTexturesInverse, (blockType.back.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x, y, z},
         {(blockType.back.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.back.texCoordVTopLeft + 1.0f) * numTexturesInverse},
         faceIndex},
        {{x, y + 1.0f, z},
         {(blockType.back.texCoordUTopLeft + 1.0f) * numTexturesInverse, (blockType.back.texCoordVTopLeft) * numTexturesInverse},
         faceIndex}};

    int backFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(backFaceIndices), std::end(backFaceIndices));
    vertices.insert(vertices.end(), std::begin(backFaceVertices), std::end(backFaceVertices));
    vertexOffset += 4;
}
} // namespace bwg