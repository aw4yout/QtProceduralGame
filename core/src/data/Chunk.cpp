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

Voxel Chunk::getVoxel(const Vector3Type localPos, const Vector3Type worldPos,
    const ContinuousDensityFuncType& getContinuousDensity) const
{
    return isValid(localPos) ? m_voxels[index(localPos)] : getContinuousDensity(worldPos.as<ValueType>());
}

constexpr std::size_t Chunk::index(const Vector3Type position)
{
    return position.x + position.y * size + position.z * size * size;
}

} // gen
