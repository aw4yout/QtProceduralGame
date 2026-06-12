#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <world/TerrainGenerator.hpp>
#include <meshing/MarchingCubes.hpp>

#include "geometry/TerrainGeometry.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    const gen::TerrainGenerator generator{{}};
    const auto chunk = generator.generate({});

    const auto mesh = gen::MarchingCubes::generateChunk(chunk);

    app::TerrainGeometry geometry;
    geometry.setMesh(mesh);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("terrainGeometry", &geometry);
    engine.loadFromModule("ProceduralGame", "Main");

    return app.exec();
}
