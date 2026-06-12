#include "noise/PerlinNoise.hpp"

namespace gen {

PerlinNoise::PerlinNoise(const SeedType seed)
    : m_perlinNoise(seed)
{}

PerlinNoise::ValueType PerlinNoise::noise(
    const ValueType x, const ValueType y,
    const int32_t octaves, const ValueType persistence) const
{
    return m_perlinNoise.octave2D_01(x, y, octaves, persistence);
}

PerlinNoise::ValueType PerlinNoise::noise(
    const ValueType x, const ValueType y, const ValueType z,
    const int32_t octaves, const ValueType persistence) const
{
    return m_perlinNoise.octave3D_01(x, y, z, octaves, persistence);
}

} // gen
