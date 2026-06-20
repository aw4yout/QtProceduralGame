#pragma once

#include "utils/Vector.hpp"

#include <array>
#include <functional>

namespace gen {

struct Voxel
{
    enum class Material : uint8_t { Air, Grass, Dirt, Stone, Sand, Snow };

    float density = -1.0f;
    Material material = Material::Air;
};

class Chunk
{
public:
    using ValueType = float;
    using Vector3Type = utils::Vector3i;
    using ContinuousDensityFuncType = std::function<Voxel(utils::Vector3<ValueType>)>;

    static constexpr uint8_t size = 16;
    static constexpr int voxelsCount = size * size * size;

public:
    explicit Chunk(Vector3Type position);

    Chunk(Chunk&&) = default;
    Chunk(const Chunk&) = delete;

    Vector3Type getPosition() const { return m_position; }
    void setPosition(const Vector3Type position) { m_position = position; }

    bool isGenerated() const { return m_generated; }
    void setGenerated(const bool generated) { m_generated = generated; }

    bool isOnlyAir() const { return m_onlyAir; }
    void setOnlyAir(const bool onlyAir) { m_onlyAir = onlyAir; }

    bool isOnlySolid() const { return m_onlySolid; }
    void setOnlySolid(const bool onlySolid) { m_onlySolid = onlySolid; }

    Voxel& getVoxel(Vector3Type position);
    const Voxel& getVoxel(Vector3Type position) const;

    Voxel getVoxel(Vector3Type localPos, Vector3Type worldPos,
        const ContinuousDensityFuncType& getContinuousDensity) const;

    static constexpr bool isValid(const Vector3Type position)
    {
        return position.x >= 0 && position.x < size
            && position.y >= 0 && position.y < size
            && position.z >= 0 && position.z < size;
    }
private:
    static constexpr std::size_t index(Vector3Type position);

private:
    Vector3Type m_position;

    std::array<Voxel, voxelsCount> m_voxels;

    bool m_generated{}, m_onlyAir{ true }, m_onlySolid{ true };
};

} // gen
