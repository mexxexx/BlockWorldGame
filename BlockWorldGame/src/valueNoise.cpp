#include "valueNoise.h"

namespace bwg
{
void ValueNoise::setupGrid(const int seed, const int gridSize, const int maxHeight)
{
    this->seed = seed;
    this->gridSize = gridSize;
    this->maxHeight = maxHeight;

    random = std::mt19937(seed);
    distribution = std::uniform_real_distribution<>(0.0, 1.0);

    grid = std::vector<float>(gridSize * gridSize);
    for (int i = 0; i < gridSize * gridSize; i++)
        grid[i] = distribution(random);
}

float ValueNoise::noise2D(const int x, const int y) const
{
    int xGrid = x % gridSize;
    if (xGrid < 0)
        xGrid += gridSize;

    int yGrid = y % gridSize;
    if (yGrid < 0)
        yGrid += gridSize;

    return grid[xGrid + yGrid * gridSize] * maxHeight;
}
} // namespace bwg