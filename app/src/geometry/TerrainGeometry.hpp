#pragma once

#include <data/Mesh.hpp>

#include <QQuick3DGeometry>

namespace app {

class TerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT
public:
    void setMesh(const gen::Mesh& mesh);

private:
    void updateData(const gen::Mesh& mesh);
};

} // app
