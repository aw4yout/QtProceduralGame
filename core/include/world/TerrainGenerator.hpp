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

        ValueType baseHeight = 32.0f;
        ValueType heightScale = 64.0f;

        int32_t octaves = 4;
        ValueType persistence = 0.5f;
        ValueType frequency = 0.5f;

        ValueType caveFrequency = 0.03f;
        ValueType caveThreshold = 0.65f;

        ValueType voxelSize = 1.0f;
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
