#include "world/TerrainGenerator.hpp"

namespace gen {

TerrainGenerator::TerrainGenerator(const Config& config)
    : m_config(config),
      m_computedSeed([&config] {
          return config.seed
              .value_or([] {
                  std::mt19937 generator{ std::random_device{}() };
                  return std::uniform_int_distribution<PerlinNoise::SeedType>{}(generator);
              }());
      }()),
      m_heightNoise(m_computedSeed),
      m_detailNoise(m_computedSeed + 1),
      m_caveNoise(m_computedSeed + 2)
{}

TerrainGenerator::ValueType TerrainGenerator::getHeight(const Vector2Type point) const
{
    constexpr auto detailMultiplier = 4.0f;
    constexpr auto mixFactor = 0.2f;

    const auto height = m_heightNoise.noise(
        point.x * m_config.frequency,
        point.y * m_config.frequency,
        m_config.octaves, m_config.persistence);
    const auto detail = m_detailNoise.noise(
        point.x * m_config.frequency * detailMultiplier,
        point.y * m_config.frequency * detailMultiplier,
        m_config.octaves, m_config.persistence);

    const auto combined = utils::mix(height, detail, mixFactor);

    return m_config.baseHeight + combined * m_config.heightScale;
}

TerrainGenerator::ValueType TerrainGenerator::getCaveDensity(const Vector3Type point) const
{
    const auto noise = m_caveNoise.noise(
        point.x * m_config.caveFrequency,
        point.y * m_config.caveFrequency,
        point.z * m_config.caveFrequency,
        m_config.octaves, m_config.persistence);

    if (noise > m_config.caveThreshold) {
        const auto depth = (noise - m_config.caveThreshold) * 2.0f;
        return -depth;
    }

    return 1.0f;
}

TerrainGenerator::ValueType TerrainGenerator::getDensity(const Vector3Type point) const
{
    const auto surfaceHeight = getHeight({ point.x, point.y });
    const auto baseDensity = surfaceHeight - point.y;

    const auto caveDensity = getCaveDensity(point);

    return std::min(baseDensity, caveDensity);
}

Chunk TerrainGenerator::generate(const Chunk::Vector3Type position) const
{
    Chunk chunk{ position };

    for (int z = 0; z < Chunk::size; ++z) {
        for (int y = 0; y < Chunk::size; ++y) {
            for (int x = 0; x < Chunk::size; ++x) {
                const Vector3Type worldPos{
                    static_cast<ValueType>(position.x * Chunk::size + x) * m_config.voxelSize,
                    static_cast<ValueType>(position.y * Chunk::size + y) * m_config.voxelSize,
                    static_cast<ValueType>(position.z * Chunk::size + z) * m_config.voxelSize,
                };

                auto& [density] = chunk.getVoxel({ x, y, z });
                density = getDensity(worldPos);
            }
        }
    }

    chunk.setGenerated(true);

    return chunk;
}

} // gen
