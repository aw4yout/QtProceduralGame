#include "world/TerrainGenerator.hpp"
#include "utils/Math.hpp"

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
      m_caveNoise(m_computedSeed + 2),
      m_temperatureNoise(m_computedSeed + 3),
      m_mountainsNoise(m_computedSeed + 4)
{}

TerrainGenerator::ValueType TerrainGenerator::getHeight(const Vector2Type point) const
{
    constexpr auto detailMultiplier = 4.0f;
    constexpr auto mountainMultiplier = 1.0f;
    constexpr auto mountainHeightMultiplier = 2.0f;
    constexpr auto detailFactor = 0.2f;

    const auto height = m_heightNoise.noise(
        point.x * m_config.frequency,
        point.y * m_config.frequency,
        m_config.octaves, m_config.persistence);
    const auto detail = m_detailNoise.noise(
        point.x * m_config.frequency * detailMultiplier,
        point.y * m_config.frequency * detailMultiplier,
        m_config.octaves, m_config.persistence);
    const auto mountainMaskNoise = m_mountainsNoise.noise(
        point.x * m_config.frequency * mountainMultiplier,
        point.y * m_config.frequency * mountainMultiplier,
        m_config.octaves, m_config.persistence);
    const auto mountainHeight = m_mountainsNoise.noise(
        point.x * m_config.frequency,
        point.y * m_config.frequency,
        m_config.octaves, m_config.persistence) * mountainHeightMultiplier;

    const auto mountainMask = utils::smoothstep(0.75f, 0.95f, mountainMaskNoise);
    const auto baseTerrain = utils::mix(height, mountainHeight, mountainMask);

    return m_config.baseHeight + (baseTerrain + detail * detailFactor) * m_config.heightScale;
}

TerrainGenerator::ValueType TerrainGenerator::getCaveDensity(const Vector3Type point) const
{
    const auto noise = m_caveNoise.noise(
        point.x * m_config.caveFrequency,
        point.y * m_config.caveFrequency,
        point.z * m_config.caveFrequency,
        m_config.octaves, m_config.persistence);

    return std::clamp((m_config.caveThreshold - noise) * 2.0f, -1.0f, 1.0f);
}

// not wasting computing power on choosing the voxel material
TerrainGenerator::ValueType TerrainGenerator::getDensity(const Vector3Type point) const
{
    const auto surfaceHeight = getHeight({ point.x, point.z });
    const auto baseDensity = surfaceHeight - point.y;

    const auto caveDensity = getCaveDensity(point);

    return utils::smoothMin(baseDensity, caveDensity, m_config.caveSmoothness);
}

// reliable material choosing
Voxel TerrainGenerator::getVoxel(const Vector3Type point) const
{
    const auto surfaceHeight = getHeight({ point.x, point.z });

    const auto baseDensity = surfaceHeight - point.y;
    const auto caveDensity = getCaveDensity(point);

    const auto density = utils::smoothMin(baseDensity, caveDensity, m_config.caveSmoothness);

    const auto material = getMaterial(point, surfaceHeight, baseDensity, density);

    return { density, material };
}

Voxel::Material TerrainGenerator::getMaterial(
    const Vector3Type point, const ValueType surfaceHeight,
    const ValueType baseDensity, const ValueType density) const
{
    if (density <= 0.0f) {
        return Voxel::Material::Air;
    }

    const auto temp = m_temperatureNoise.noise(point.x * 0.01f, point.z * 0.01f);
    const auto isDesert = temp >= 0.65f;

    if (surfaceHeight > m_config.snowLine) {
        return baseDensity < 3.0f && !isDesert ? Voxel::Material::Snow : Voxel::Material::Stone;
    }

    if (isDesert) {
        if (baseDensity < m_config.sandDepth) {
            return Voxel::Material::Sand;
        }
        return Voxel::Material::Stone;
    }

    if (baseDensity < m_config.grassDepth) {
        return Voxel::Material::Grass;
    }
    if (baseDensity < m_config.dirtDepth) {
        return Voxel::Material::Dirt;
    }

    return Voxel::Material::Stone;
}

Chunk TerrainGenerator::generate(const Chunk::Vector3Type position) const
{
    Chunk chunk{ position };

    for (int z = 0; z < Chunk::size; ++z) {
        for (int y = 0; y < Chunk::size; ++y) {
            for (int x = 0; x < Chunk::size; ++x) {
                const auto worldPos = position * Chunk::size + utils::Vector3{ x, y, z };
                chunk.getVoxel({ x, y, z }) = getVoxel(worldPos.as<ValueType>());
            }
        }
    }

    chunk.setGenerated(true);

    return chunk;
}

} // gen
