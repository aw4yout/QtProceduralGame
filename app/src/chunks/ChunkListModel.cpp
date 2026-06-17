#include "ChunkListModel.hpp"
#include "TerrainGeometry.hpp"

namespace app {

int ChunkListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(m_chunks.size());
}

QVariant ChunkListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_chunks.size())) {
        return {};
    }
    if (role == GeometryRole) {
        return QVariant::fromValue(m_chunks[index.row()]);
    }
    return {};
}

QHash<int, QByteArray> ChunkListModel::roleNames() const
{
    return { { GeometryRole, "geometry" } };
}

void ChunkListModel::addChunks(const std::vector<TerrainGeometry*>& chunks)
{
    beginInsertRows({}, static_cast<int>(m_chunks.size()), static_cast<int>(m_chunks.size() + chunks.size() - 1));
    m_chunks.append_range(chunks);
    endInsertRows();
}

void ChunkListModel::removeChunk(TerrainGeometry* chunk)
{
    if (const auto it = std::ranges::find(m_chunks, chunk);
        it != m_chunks.end()) {
        const int index = static_cast<int>(std::distance(m_chunks.begin(), it));
        beginRemoveRows(QModelIndex(), index, index);
        m_chunks.erase(it);
        endRemoveRows();
    }
}

} // app
