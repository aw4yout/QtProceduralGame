#pragma once

#include "utils/Vector.hpp"

#include <array>
#include <functional>

namespace gen {

struct Voxel
{
    float density = -1.0f;
};

class Chunk
{
public:
    using ValueType = float;
    using Vector3Type = utils::Vector3i;
    using ContinuousDensityFuncType = std::function<ValueType(utils::Vector3<ValueType>)>;

    static constexpr int size = 32;
    static constexpr int voxelsCount = size * size * size;

public:
    explicit Chunk(Vector3Type position);

    Chunk(Chunk&&) = default;
    Chunk(const Chunk&) = delete;

    Vector3Type getPosition() const { return m_position; }
    void setPosition(const Vector3Type position) { m_position = position; }

    bool isGenerated() const { return m_generated; }
    void setGenerated(const bool generated) { m_generated = generated; }

    Voxel& getVoxel(Vector3Type position);
    const Voxel& getVoxel(Vector3Type position) const;

    ValueType getDensityOrDefault(Vector3Type position, ValueType value = 0.0f) const;
    ValueType getDensityOrDefault(
        Vector3Type localPos, Vector3Type worldPos,
        const ContinuousDensityFuncType& getContinuousDensity) const;

private:
    static constexpr bool isValid(Vector3Type position);
    static constexpr std::size_t index(Vector3Type position);

private:
    Vector3Type m_position;

    std::array<Voxel, voxelsCount> m_voxels;

    bool m_generated{}, m_meshed{};
};

} // gen
