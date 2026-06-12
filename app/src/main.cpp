#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include <world/TerrainGenerator.hpp>
#include <meshing/MarchingCubes.hpp>

#include "geometry/TerrainGeometry.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    const gen::TerrainGenerator generator{{}};
    const auto getDensity = [&generator](const gen::Mesh::Vector3Type pos) {
        return generator.getDensity(pos);
    };

    std::array<std::unique_ptr<app::TerrainGeometry>, 512> chunksArray;

    QVariantList chunks;

    size_t index{};
    for (int i{}; i < 16; ++i) {
        for (int j{}; j < 2; ++j) {
            for (int k{}; k < 16; ++k) {
                const auto chunk = generator.generate({ k, 0 - j, i });

                const auto mesh = gen::MarchingCubes::generateChunk(
                    chunk, getDensity);

                auto geometry = std::make_unique<app::TerrainGeometry>();
                geometry->setMesh(mesh);

                chunks.append(QVariant::fromValue(geometry.get()));

                chunksArray[index++] = std::move(geometry);
            }
        }
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chunks", chunks);
    engine.loadFromModule("ProceduralGame", "Main");

    return app.exec();
}
