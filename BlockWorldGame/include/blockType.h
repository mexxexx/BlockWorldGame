#ifndef __BLOCKENGINE_BLOCKTYPE_H__
#define __BLOCKENGINE_BLOCKTYPE_H__

namespace bwg
{
struct BlockType
{
    const static int NUMBER_OF_BLOCK_TYPES = 2;

    enum BlockTypeID : int 
    {
        Air = 0,
        Grass,
    };

    enum BlockTypeVisibility {
        Nothing,
        Solid,
        Transparent
    };

    struct Face
    {
        float texCoordUTopLeft;
        float texCoordVTopLeft;
    };

    BlockTypeID id;
    BlockTypeVisibility visibility;
    Face front;
    Face back;
    Face top;
    Face bottom;
    Face left;
    Face right;
};
} // namespace bwg

#endif