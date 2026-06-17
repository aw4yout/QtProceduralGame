#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <world/TerrainGenerator.hpp>
#include <meshing/MarchingCubes.hpp>

#include "geometry/TerrainGeometry.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("ProceduralGame", "Main");

    return app.exec();
}
