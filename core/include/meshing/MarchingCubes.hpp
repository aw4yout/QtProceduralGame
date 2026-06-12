#pragma once

#include "data/Chunk.hpp"
#include "data/Mesh.hpp"

#include <functional>

namespace gen {

class MarchingCubes
{
public:
    using ValueType = Chunk::ValueType;
    using DensityFuncType = std::function<ValueType(Chunk::Vector3Type worldPos)>;

    static Mesh generateChunk(const Chunk& chunk, ValueType isoLevel = {});

private:
    static ValueType getOffset(
        ValueType x, ValueType y, ValueType isoLevel);

    static Mesh::Vector3Type computeNormal(
        Chunk::Vector3Type worldPos, Mesh::Vector3Type vertexPos,
        const DensityFuncType& getDensity);
};

} // gen
