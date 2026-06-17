#pragma once

#include <data/Mesh.hpp>

#include <QQuick3DGeometry>
#include <QVector3D>

namespace app {

class TerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(quint64 vertexCount READ vertexCount NOTIFY vertexCountChanged)
    Q_PROPERTY(QVector3D chunkPosition READ chunkPosition CONSTANT)
public:
    void setMesh(const gen::Mesh& mesh);

    quint64 vertexCount() const { return m_vertexCount; }

    QVector3D chunkPosition() const { return m_chunkPosition; }
    void setChunkPosition(const QVector3D& position) { m_chunkPosition = position; }

signals:
    void vertexCountChanged();

private:
    void updateData(const gen::Mesh& mesh);

private:
    quint64 m_vertexCount{};
    QVector3D m_chunkPosition;
};

} // app
