#include "data/Mesh.hpp"

#include <ranges>

namespace gen {

void Mesh::addTriangle(const std::array<Vertex, 3>& triangle)
{
    const auto baseIndex = static_cast<uint32_t>(m_vertices.size());
    m_vertices.append_range(triangle);
    m_indices.append_range(std::views::iota(baseIndex, baseIndex + 3));
}

void Mesh::clear()
{
    m_vertices.clear();
    m_indices.clear();
}

} // gen
