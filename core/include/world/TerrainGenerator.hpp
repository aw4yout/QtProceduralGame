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
        ValueType heightScale = 32.0f;

        int32_t octaves = 6;
        ValueType persistence = 0.4f;
        ValueType frequency = 0.01f;

        ValueType caveFrequency = 0.04f;
        ValueType caveThreshold = 0.7f;
        ValueType caveSmoothness = 0.1f;

        ValueType snowLine = 40.0f;
        ValueType sandDepth = 3.0f;
        ValueType grassDepth = 1.0f;
        ValueType dirtDepth = 3.5f;
    };

    explicit TerrainGenerator(const Config& config);

    ValueType getHeight(Vector2Type point) const;
    ValueType getCaveDensity(Vector3Type point) const;
    ValueType getDensity(Vector3Type point) const;
    Voxel getVoxel(Vector3Type point) const;
    Voxel::Material getMaterial(Vector3Type point, ValueType surfaceHeight, ValueType baseDensity, ValueType density) const;

    Chunk generate(Chunk::Vector3Type position) const;
    void regenerateIgnoreAir(Chunk& chunk) const;

private:
    Config m_config;

    PerlinNoise::SeedType m_computedSeed;

    PerlinNoise m_heightNoise, m_detailNoise, m_caveNoise;
    PerlinNoise m_temperatureNoise, m_mountainsNoise;
};

} // gen
