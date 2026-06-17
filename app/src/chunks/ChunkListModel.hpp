#pragma once
#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

namespace app {

class TerrainGeometry;

class ChunkListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles { GeometryRole = Qt::UserRole + 1 };

    explicit ChunkListModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addChunks(const std::vector<TerrainGeometry*>& chunks);
    void removeChunk(TerrainGeometry* chunk);

private:
    std::vector<TerrainGeometry*> m_chunks;
};

} // app
