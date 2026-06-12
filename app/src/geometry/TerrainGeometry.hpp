#pragma once

#include <data/Mesh.hpp>

#include <QQuick3DGeometry>

namespace app {

class TerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(quint64 vertexCount READ vertexCount NOTIFY vertexCountChanged)
public:
    void setMesh(const gen::Mesh& mesh);

    quint64 vertexCount() const { return m_vertexCount; }

signals:
    void vertexCountChanged();

private:
    void updateData(const gen::Mesh& mesh);

private:
    quint64 m_vertexCount;
};

} // app
