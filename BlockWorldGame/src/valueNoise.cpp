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

float ValueNoise::noise2D(const float x, const float y) const
{
    float result = 0;
    float maxAmplitude = 0;
    int octaves = 2;
    float amplitude = 3;
    float frequency = 0.1;

    float persistence = 0.7;
    float lacunarity = 2;

    for (int i = 0; i < octaves; i++)
    {
        result += singleOctaveCubicNoise(x * frequency, y * frequency) * amplitude;

        maxAmplitude += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }
    result /= maxAmplitude;

    return result * maxHeight;
}

float ValueNoise::singleOctaveValueNoise(const float x, const float y) const
{
    int ix0 = fastFloor(x);
    int iy0 = fastFloor(y);

    float fx0 = x - ix0;
    float fy0 = y - iy0;

    float t = fade(fx0);
    float s = fade(fy0);

    float nx0 = gridValue(ix0, iy0);
    float nx1 = gridValue(ix0 + 1, iy0);
    float n0 = lerp(t, nx0, nx1);

    nx0 = gridValue(ix0, iy0 + 1);
    nx1 = gridValue(ix0 + 1, iy0 + 1);
    float n1 = lerp(t, nx0, nx1);

    return lerp(s, n0, n1);
}

float ValueNoise::singleOctaveCubicNoise(const float x, const float y) const
{
    const int ix = fastFloor(x);
    const int iy = fastFloor(y);

    const float fx = x - ix;
    const float fy = y - iy;

    float xSamples[4];

    for (int i = 0; i < 4; i++)
    {
        xSamples[i] = cubicInterpolate(
            gridValue(ix - 1, iy - 1 + i),
            gridValue(ix, iy - 1 + i),
            gridValue(ix + 1, iy - 1 + i),
            gridValue(ix + 2, iy - 1 + i),
            fx);
    }

    return cubicInterpolate(
               xSamples[0], xSamples[1], xSamples[2], xSamples[3], fy) *
               0.5f +
           0.25f;
}

int inline ValueNoise::fastFloor(const float x) const
{
    int r = (int)x;
    if (x >= 0 || r == x)
        return r;
    else
        return r - 1;
}

float ValueNoise::gridValue(const int x, const int y) const
{
    int xGrid = x % gridSize;
    if (xGrid < 0)
        xGrid += gridSize;

    int yGrid = y % gridSize;
    if (yGrid < 0)
        yGrid += gridSize;

    return grid[xGrid + yGrid * gridSize];
}

float ValueNoise::lerp(const float amount, const float a, const float b) const
{
    return (1 - amount) * a + amount * b;
}

float ValueNoise::fade(const float t) const
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float ValueNoise::cubicInterpolate(
    const float a,
    const float b,
    const float c,
    const float d,
    const float amount) const
{
    const float p = (d - c) - (a - b);

    return amount * (amount * (amount * p + ((a - b) - p)) + (c - a)) + b;
}
} // namespace bwg