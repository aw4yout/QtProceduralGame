#pragma once
#include "ChunkListModel.hpp"

#include <data/Chunk.hpp>

#include <QVector3D>
#include <QtTaskTree>
#include <QtQml/qqmlregistration.h>

#include <unordered_map>
#include <memory>

#include "world/TerrainGenerator.hpp"

namespace gen {
class TerrainGenerator;
}

namespace app {

class TerrainGeometry;

struct ChunkData
{
    enum class State : uint8_t { Unloaded, Generating, Generated, Meshing, Meshed };

    gen::Chunk::Vector3Type position{};
    State state = State::Unloaded;

    std::shared_ptr<TerrainGeometry> geometry;
    std::shared_ptr<gen::Chunk> chunk;

    std::atomic_bool cancelled{};
};

class ChunkManager : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

    Q_PROPERTY(ChunkListModel* activeChunks READ activeChunks CONSTANT)
    Q_PROPERTY(QVector3D playerPosition READ playerPosition WRITE setPlayerPosition)

    Q_PROPERTY(uint8_t worldScale READ worldScale CONSTANT)
    Q_PROPERTY(uint8_t chunkSize READ chunkSize CONSTANT)
public:
    using ChunkDataPtrType = std::shared_ptr<ChunkData>;
    using ChunkMapType = std::unordered_map<gen::Chunk::Vector3Type, ChunkDataPtrType, gen::utils::Vector3iHash>;

    explicit ChunkManager(QObject* parent = nullptr);

    ChunkListModel* activeChunks() { return &m_activeChunks; }

    QVector3D playerPosition() const { return m_playerPos; }
    void setPlayerPosition(const QVector3D& position) { m_playerPos = position; }

    static constexpr uint8_t worldScale() { return 1; }
    static constexpr uint8_t chunkSize() { return gen::Chunk::size; }

    Q_INVOKABLE void update(uint8_t chunksCount = 2);
    Q_INVOKABLE void setVoxel(const QVector3D& voxelPos, bool solid = false);

    gen::Voxel getVoxelFromLoadedChunks(
        const gen::utils::Vector3<gen::Chunk::ValueType>& worldPos) const;

private:
    void finishGeneration(ChunkDataPtrType generatedChunk);
    void unload(gen::Chunk::Vector3Type position);

    QtTaskTree::Group generateChunksTask(const QList<ChunkDataPtrType>& chunks) const;
    QtTaskTree::Group generateMeshesTask(const QList<ChunkDataPtrType>& chunks);

    QtTaskTree::ExecutableItem recipe(
        const QList<ChunkDataPtrType>& renderChunks,
        const QList<ChunkDataPtrType>& borderChunks);

private:
    ChunkMapType m_chunks;
    ChunkListModel m_activeChunks;
    QVector3D m_playerPos;

    uint8_t m_renderDistance = 2;

    std::shared_ptr<const gen::TerrainGenerator> m_generator;
};

} // app
