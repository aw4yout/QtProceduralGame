#pragma once

#include "data/Chunk.hpp"
#include "data/Mesh.hpp"

#include <functional>

namespace gen {

class MarchingCubes
{
public:
    using ValueType = Chunk::ValueType;
    using DensityFuncType = std::function<ValueType(Chunk::Vector3Type)>;
    using ContinuousDensityFuncType = std::function<Voxel(Mesh::Vector3Type)>;

    static Mesh generateChunk(
        const Chunk& chunk,
        const ContinuousDensityFuncType& getContinuousVoxel,
        ValueType isoLevel = {});

private:
    static ValueType getOffset(
        ValueType x, ValueType y, ValueType isoLevel);

    static Mesh::Vector3Type computeNormal(
        Mesh::Vector3Type vertexPos,
        const ContinuousDensityFuncType& getContinuousVoxel);
};

} // gen
