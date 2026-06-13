#include "TerrainGeometry.hpp"

#include <QVector3D>

namespace app {

void TerrainGeometry::setMesh(const gen::Mesh& mesh)
{
    updateData(mesh);
    update();
}

void TerrainGeometry::updateData(const gen::Mesh& mesh)
{
    clear();

    m_vertexCount = mesh.getVertices().size();
    emit vertexCountChanged();

    constexpr auto stride = sizeof(gen::Mesh::Vertex);

    const QByteArray vertexData(
        reinterpret_cast<const char*>(mesh.getVertices().data()),
        static_cast<qsizetype>(mesh.getVertices().size() * stride));

    const QByteArray indexData(
        reinterpret_cast<const char*>(mesh.getIndices().data()),
        static_cast<qsizetype>(mesh.getIndices().size() * sizeof(uint32_t)));

    setVertexData(vertexData);
    setIndexData(indexData);
    setStride(stride);

    setPrimitiveType(PrimitiveType::Triangles);

    addAttribute(Attribute::PositionSemantic, 0, Attribute::F32Type);
    addAttribute(Attribute::NormalSemantic, sizeof(gen::Mesh::Vector3Type), Attribute::F32Type);
    addAttribute(Attribute::TangentSemantic, 2 * sizeof(gen::Mesh::Vector3Type), Attribute::F32Type);
    addAttribute(Attribute::IndexSemantic, 0, Attribute::U32Type);

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& [position, _, $] : mesh.getVertices()) {
        minX = qMin(minX, position.x); maxX = qMax(maxX, position.x);
        minY = qMin(minY, position.y); maxY = qMax(maxY, position.y);
        minZ = qMin(minZ, position.z); maxZ = qMax(maxZ, position.z);
    }

    setBounds({ minX, minY, minZ }, { maxX, maxY, maxZ });
}

} // app
