#ifndef __BLOCKWORLDGAME_BLOCKWORLDGAME_H__
#define __BLOCKWORLDGAME_BLOCKWORLDGAME_H__

#include "engine.h"

#include "terrain.h"

namespace bwg
{
class BWG : public engine::Game
{
protected:
    void initialize();
    void loadContent();
    void unloadContent();
    void update(const double timeStep);
    void draw();

private:
    void cameraMovement(const double deltaTime);
    Terrain terrain;
    engine::Camera camera;
    double cameraMovementSpeed = 5;
    double cameraRotationSpeed = 0.1;
    bool firstMouse = true;
    int lastX, lastY;
};
} // namespace bwg

#endif