#include "chunk.h"

namespace bwg
{
BlockType Chunk::blockTypes[] = {};
Chunk::Chunk(const int chunkOffsetX, const int chunkOffsetY, const int chunkOffsetZ) : chunkOffsetX(chunkOffsetX), chunkOffsetY(chunkOffsetY), chunkOffsetZ(chunkOffsetZ)
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

void Chunk::generateChunk()
{
    for (int x = 0; x < CHUNK_DIMENSION; x++)
    {
        for (int y = 0; y < CHUNK_DIMENSION; y++)
        {
            for (int z = 0; z < CHUNK_DIMENSION; z++)
            {
                blocks[x + y * CHUNK_DIMENSION + z * CHUNK_DIMENSION * CHUNK_DIMENSION] = (-(x + y - z) % 3 == 0) ? 1 : 1;
            }
        }
    }
}

bool Chunk::blockIsSeeThrough(const BlockType *block) const
{
    return block && (block->visibility == BlockType::Nothing || block->visibility == BlockType::Transparent);
}

void Chunk::getVisibleFaces(std::vector<GLfloat> &vertices, std::vector<GLint> &indices, int &vertexOffset) const
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

                    neighbour = getBlockType(x, y, z + 1);
                    if (blockIsSeeThrough(neighbour))
                        addBlockFrontFace(vertices, vertexOffset, indices, xWorldPos, yWorldPos, zWorldPos, blockType);
                }
                else if (blockIsSeeThrough(&blockType))
                {
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
        return nullptr;
}

void Chunk::addBlockFrontFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float frontFaceVertices[] = {
        x, y, z + 1.0f, (blockType.front.texCoordUTopLeft) / 16.0f, (blockType.front.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y, z + 1.0f, (blockType.front.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.front.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y + 1.0f, z + 1.0f, (blockType.front.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.front.texCoordVTopLeft) / 16.0f,
        x, y + 1.0f, z + 1.0f, (blockType.front.texCoordUTopLeft) / 16.0f, (blockType.front.texCoordVTopLeft) / 16.0f};

    int frontFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(frontFaceIndices), std::end(frontFaceIndices));
    vertices.insert(vertices.end(), std::begin(frontFaceVertices), std::end(frontFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockBackFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float backFaceVertices[]{
        x + 1.0f, y + 1.0f, z, (blockType.back.texCoordUTopLeft) / 16.0f, (blockType.back.texCoordVTopLeft) / 16.0f,
        x + 1.0f, y, z, (blockType.back.texCoordUTopLeft) / 16.0f, (blockType.back.texCoordVTopLeft + 1.0f) / 16.0f,
        x, y, z, (blockType.back.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.back.texCoordVTopLeft + 1.0f) / 16.0f,
        x, y + 1.0f, z, (blockType.back.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.back.texCoordVTopLeft) / 16.0f};

    int backFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(backFaceIndices), std::end(backFaceIndices));
    vertices.insert(vertices.end(), std::begin(backFaceVertices), std::end(backFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockBottomFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float bottomFaceVertices[]{
        x, y, z, (blockType.bottom.texCoordUTopLeft) / 16.0f, (blockType.bottom.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y, z, (blockType.bottom.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.bottom.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y, z + 1.0f, (blockType.bottom.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.bottom.texCoordVTopLeft) / 16.0f,
        x, y, z + 1.0f, (blockType.bottom.texCoordUTopLeft) / 16.0f, (blockType.bottom.texCoordVTopLeft) / 16.0f};

    int bottomFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(bottomFaceIndices), std::end(bottomFaceIndices));
    vertices.insert(vertices.end(), std::begin(bottomFaceVertices), std::end(bottomFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockTopFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float topFaceVertices[]{
        x + 1.0f, y + 1.0f, z + 1.0f, (blockType.top.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.top.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y + 1.0f, z, (blockType.top.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.top.texCoordVTopLeft) / 16.0f,
        x, y + 1.0f, z, (blockType.top.texCoordUTopLeft) / 16.0f, (blockType.top.texCoordVTopLeft) / 16.0f,
        x, y + 1.0f, z + 1.0f, (blockType.top.texCoordUTopLeft) / 16.0f, (blockType.top.texCoordVTopLeft + 1.0f) / 16.0f};

    int topFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(topFaceIndices), std::end(topFaceIndices));
    vertices.insert(vertices.end(), std::begin(topFaceVertices), std::end(topFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockLeftFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float leftFaceVertices[]{
        x, y + 1.0f, z + 1.0f, (blockType.left.texCoordUTopLeft) / 16.0f, (blockType.left.texCoordVTopLeft) / 16.0f,
        x, y + 1.0f, z, (blockType.left.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.left.texCoordVTopLeft) / 16.0f,
        x, y, z, (blockType.left.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.left.texCoordVTopLeft + 1.0f) / 16.0f,
        x, y, z + 1.0f, (blockType.left.texCoordUTopLeft) / 16.0f, (blockType.left.texCoordVTopLeft + 1.0f) / 16.0f};

    int leftFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(leftFaceIndices), std::end(leftFaceIndices));
    vertices.insert(vertices.end(), std::begin(leftFaceVertices), std::end(leftFaceVertices));
    vertexOffset += 4;
}

void Chunk::addBlockRightFace(std::vector<GLfloat> &vertices, int &vertexOffset, std::vector<GLint> &indices, const float x, const float y, const float z, const BlockType &blockType)
{
    float rightFaceVertices[]{
        x + 1.0f, y, z, (blockType.right.texCoordUTopLeft) / 16.0f, (blockType.right.texCoordVTopLeft + 1.0f) / 16.0f,
        x + 1.0f, y + 1.0f, z, (blockType.right.texCoordUTopLeft) / 16.0f, (blockType.right.texCoordVTopLeft) / 16.0f,
        x + 1.0f, y + 1.0f, z + 1.0f, (blockType.right.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.right.texCoordVTopLeft) / 16.0f,
        x + 1.0f, y, z + 1.0f, (blockType.right.texCoordUTopLeft + 1.0f) / 16.0f, (blockType.right.texCoordVTopLeft + 1.0f) / 16.0f};

    int rightFaceIndices[] = {
        vertexOffset + 0, vertexOffset + 1, vertexOffset + 2, // first traingle
        vertexOffset + 2, vertexOffset + 3, vertexOffset + 0, // second traingle
    };

    indices.insert(indices.end(), std::begin(rightFaceIndices), std::end(rightFaceIndices));
    vertices.insert(vertices.end(), std::begin(rightFaceVertices), std::end(rightFaceVertices));
    vertexOffset += 4;
}
} // namespace bwg