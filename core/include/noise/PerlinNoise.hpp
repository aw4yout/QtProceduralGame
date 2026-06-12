#pragma once

#include <PerlinNoise/PerlinNoise.hpp>

namespace gen {

class PerlinNoise
{
public:
    using ValueType = float;
    using SeedType = siv::PerlinNoise::seed_type;

    explicit PerlinNoise(SeedType seed);

    ValueType noise(ValueType x, ValueType y, int32_t octaves = 1, ValueType persistence = 0.5f) const;
    ValueType noise(ValueType x, ValueType y, ValueType z, int32_t octaves = 1, ValueType persistence = 0.5f) const;

private:
    siv::BasicPerlinNoise<ValueType> m_perlinNoise;
};

} // gen
