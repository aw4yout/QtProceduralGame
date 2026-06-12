#include "data/Chunk.hpp"

#include <cassert>

namespace gen {

Chunk::Chunk(const Vector3Type position)
    : m_position(position)
{}

Voxel& Chunk::getVoxel(const Vector3Type position)
{
#ifndef NDEBUG
    assert(isValid(position) && "coords OOB");
#endif
    return m_voxels[index(position)];
}

const Voxel& Chunk::getVoxel(const Vector3Type position) const
{
#ifndef NDEBUG
    assert(isValid(position) && "coords OOB");
#endif
    return m_voxels[index(position)];
}

Chunk::ValueType Chunk::getDensityOrDefault(
    const Vector3Type position, const ValueType value) const
{
    return isValid(position) ? m_voxels[index(position)].density : value;
}

constexpr bool Chunk::isValid(const Vector3Type position)
{
    return position.x >= 0 && position.x < size
        && position.y >= 0 && position.y < size
        && position.z >= 0 && position.z < size;
}

constexpr std::size_t Chunk::index(const Vector3Type position)
{
    return position.x + position.y * size + position.z * size * size;
}

} // gen
