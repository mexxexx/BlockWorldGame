#ifndef __BLOCKWORLDGAME_BLOCKWORLDGAME_H__
#define __BLOCKWORLDGAME_BLOCKWORLDGAME_H__

#include "engine.h"
namespace bwg
{
class BWG : public engine::Game
{
protected:
    void loadContent();
    void unloadContent();
    void update(double timeStep);
    void draw();
};
} // namespace bwg

#endif