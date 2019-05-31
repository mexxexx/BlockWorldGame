#ifndef __BLOCKWORLDGAME_VALUENOISE_H__
#define __BLOCKWORLDGAME_VALUENOISE_H__

#include <random>

namespace bwg
{
class ValueNoise
{
    public:
        static const int DEFAULT_GRID_SIZE = 256;
        float noise2D(const int x, const int y) const;
        void setupGrid(const int seed, const int gridSize, const int maxHeight);
    private:
        std::vector<float> grid;
        int maxHeight;
        int gridSize;
        int seed;
        std::mt19937 random;
        std::uniform_real_distribution<> distribution;
};
} // namespace bwg

#endif