#pragma once

#include <utils/Vector.hpp>

#include <array>
#include <vector>

namespace gen {

class Mesh
{
public:
    using ValueType = float;
    using Vector3Type = utils::Vector3<ValueType>;

    struct Vertex
    {
        Vector3Type position, normal;
        Vector3Type materialProperties; // primaryMat, secondaryMat, blendWeight
    };

    void addTriangle(const std::array<Vertex, 3>& triangle);

    void clear();

    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<uint32_t>& getIndices() const { return m_indices; }

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};

} // gen
