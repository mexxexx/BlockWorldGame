#ifndef __BLOCKWORLDGAME_VALUENOISE_H__
#define __BLOCKWORLDGAME_VALUENOISE_H__

#include <random>

namespace bwg
{
class ValueNoise
{
    public:
        static const int DEFAULT_GRID_SIZE = 1024;
        float noise2D(const float x, const float y) const;
        void setupGrid(const int seed, const int gridSize, const int maxHeight);
    private:
        static const int primeOffsetX;
        static const int primeOffsetY;
        std::vector<float> grid;
        int maxHeight;
        int gridSize;
        int seed;
        std::mt19937 random;
        std::uniform_real_distribution<> distribution;
        float singleOctaveValueNoise(const float x, const float y) const;
        float singleOctaveCubicNoise(const float x, const float y) const;
        float gridValue(const int x, const int y) const;
        int inline fastFloor(const float x) const;
        float lerp(const float amount, const float a, const float b) const;
        float fade(const float t) const;
        float cubicInterpolate(const float a, const float b, const float c, const float d, const float amount) const;
};
} // namespace bwg

#endif