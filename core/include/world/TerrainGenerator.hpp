#pragma once

#include "noise/PerlinNoise.hpp"
#include "utils/Vector.hpp"
#include "data/Chunk.hpp"

#include <optional>

namespace gen {

class TerrainGenerator
{
public:
    using ValueType = PerlinNoise::ValueType;
    using Vector2Type = utils::Vector2<ValueType>;
    using Vector3Type = utils::Vector3<ValueType>;

    struct Config
    {
        std::optional<PerlinNoise::SeedType> seed = std::nullopt;

        ValueType baseHeight = 0.0f;
        ValueType heightScale = 30.0f;

        int32_t octaves = 4;
        ValueType persistence = 0.4f;
        ValueType frequency = 0.01f;

        ValueType caveFrequency = 0.03f;
        ValueType caveThreshold = 0.8f;
        ValueType caveSmoothness = 0.3f;
    };

    explicit TerrainGenerator(const Config& config);

    ValueType getHeight(Vector2Type point) const;
    ValueType getCaveDensity(Vector3Type point) const;
    ValueType getDensity(Vector3Type point) const;

    Chunk generate(Chunk::Vector3Type position) const;

private:
    Config m_config;

    PerlinNoise::SeedType m_computedSeed;

    PerlinNoise m_heightNoise, m_detailNoise, m_caveNoise;
};

} // gen
