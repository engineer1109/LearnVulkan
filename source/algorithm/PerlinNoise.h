//
// Created by wjl on 2023/1/29.
//

#ifndef LEARNVULKAN_PERLINNOISE_H
#define LEARNVULKAN_PERLINNOISE_H

#include "base_template.h"

BEGIN_NAMESPACE(VulkanEngine)

template <typename T>
class PerlinNoise
{
private:
    uint32_t permutations[512];
    T fade(T t)
    {
        return t * t * t * (t * (t * (T)6 - (T)15) + (T)10);
    }
    T lerp(T t, T a, T b)
    {
        return a + t * (b - a);
    }
    T grad(int hash, T x, T y, T z)
    {
        // Convert LO 4 bits of hash code into 12 gradient directions
        int h = hash & 15;
        T u = h < 8 ? x : y;
        T v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
public:
    PerlinNoise()
    {
        // Generate random lookup for permutations containing all numbers from 0..255
        std::vector<uint8_t> plookup;
        plookup.resize(256);
        std::iota(plookup.begin(), plookup.end(), 0);
        std::default_random_engine rndEngine(std::random_device{}());
        std::shuffle(plookup.begin(), plookup.end(), rndEngine);

        for (uint32_t i = 0; i < 256; i++)
        {
            permutations[i] = permutations[256 + i] = plookup[i];
        }
    }
    T noise(T x, T y, T z)
    {
        // Find unit cube that contains point
        int32_t X = (int32_t)floor(x) & 255;
        int32_t Y = (int32_t)floor(y) & 255;
        int32_t Z = (int32_t)floor(z) & 255;
        // Find relative x,y,z of point in cube
        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        // Compute fade curves for each of x,y,z
        T u = fade(x);
        T v = fade(y);
        T w = fade(z);

        // Hash coordinates of the 8 cube corners
        uint32_t A = permutations[X] + Y;
        uint32_t AA = permutations[A] + Z;
        uint32_t AB = permutations[A + 1] + Z;
        uint32_t B = permutations[X + 1] + Y;
        uint32_t BA = permutations[B] + Z;
        uint32_t BB = permutations[B + 1] + Z;

        // And add blended results for 8 corners of the cube;
        T res = lerp(w, lerp(v,
                             lerp(u, grad(permutations[AA], x, y, z), grad(permutations[BA], x - 1, y, z)), lerp(u, grad(permutations[AB], x, y - 1, z), grad(permutations[BB], x - 1, y - 1, z))),
                     lerp(v, lerp(u, grad(permutations[AA + 1], x, y, z - 1), grad(permutations[BA + 1], x - 1, y, z - 1)), lerp(u, grad(permutations[AB + 1], x, y - 1, z - 1), grad(permutations[BB + 1], x - 1, y - 1, z - 1))));
        return res;
    }
};

// Fractal noise generator based on perlin noise above
template <typename T>
class FractalNoise
{
private:
    PerlinNoise<float> perlinNoise;
    uint32_t octaves;
    T frequency;
    T amplitude;
    T persistence;
public:

    FractalNoise(const PerlinNoise<T> &perlinNoise)
    {
        this->perlinNoise = perlinNoise;
        octaves = 6;
        persistence = (T)0.5;
    }

    T noise(T x, T y, T z)
    {
        T sum = 0;
        T frequency = (T)1;
        T amplitude = (T)1;
        T max = (T)0;
        for (int32_t i = 0; i < octaves; i++)
        {
            sum += perlinNoise.noise(x * frequency, y * frequency, z * frequency) * amplitude;
            max += amplitude;
            amplitude *= persistence;
            frequency *= (T)2;
        }

        sum = sum / max;
        return (sum + (T)1.0) / (T)2.0;
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_PERLINNOISE_H
