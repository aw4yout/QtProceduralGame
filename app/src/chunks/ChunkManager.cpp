#include "ChunkManager.hpp"
#include "TerrainGeometry.hpp"

#include <world/TerrainGenerator.hpp>
#include <meshing/MarchingCubes.hpp>

#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>

namespace app {

ChunkManager::ChunkManager(QObject* parent)
    : QObject(parent)
{
    m_generator = std::make_shared<gen::TerrainGenerator>(gen::TerrainGenerator::Config{});
}

void ChunkManager::update(const uint8_t chunksCount)
{
    const gen::Chunk::Vector3Type playerChunk{
        static_cast<int>(std::floor(m_playerPos.x() / gen::Chunk::size / worldScale())),
        static_cast<int>(std::floor(m_playerPos.y() / gen::Chunk::size / worldScale())),
        static_cast<int>(std::floor(m_playerPos.z() / gen::Chunk::size / worldScale()))
    };

    std::vector<gen::Chunk::Vector3Type> toUnload;
    for (const auto& position : m_chunks | std::views::keys) {
        const auto dist = std::max({
            std::abs(position.x - playerChunk.x),
            std::abs(position.y - playerChunk.y) * 2,
            std::abs(position.z - playerChunk.z)
        });

        if (dist > m_renderDistance) {
            toUnload.push_back(position);
        }
    }

    for (const auto& position : toUnload) {
        unload(position);
    }

    size_t processed{};
    for (int32_t radius = 0; radius <= m_renderDistance && processed < chunksCount; ++radius) {
        for (int32_t x = -radius; x <= radius && processed < chunksCount; ++x) {
            for (int32_t y = -radius / 2; y <= radius / 2 && processed < chunksCount; ++y) {
                for (int32_t z = -radius; z <= radius && processed < chunksCount; ++z) {
                    if (processed >= chunksCount) {
                        return;
                    }
                    if (const auto target = playerChunk + gen::utils::Vector3{ x, y, z };
                        !m_chunks.contains(target) && jobsCount < std::thread::hardware_concurrency()) {
                        ++jobsCount;
                        generate(target);
                        processed++;
                    }
                }
            }
        }
    }
}

void ChunkManager::destroyVoxel(const QVector3D& voxelPos, const QVector3D& chunkPos)
{
    const gen::Chunk::Vector3Type target{
        static_cast<int>(chunkPos.x()),
        static_cast<int>(chunkPos.y()),
        static_cast<int>(chunkPos.z())
    };

    const auto it = m_chunks.find(target);
    if (it == m_chunks.end() || !it->second.chunk) {
        return;
    }

    const gen::Chunk::Vector3Type localPos{
        static_cast<int>(std::floor(voxelPos.x())) - target.x * gen::Chunk::size,
        static_cast<int>(std::floor(voxelPos.y())) - target.y * gen::Chunk::size,
        static_cast<int>(std::floor(voxelPos.z())) - target.z * gen::Chunk::size
    };

    it->second.chunk->getVoxel(localPos) = { -1.0f, gen::Voxel::Material::Air };

    regenerate(target);

    const std::array<std::pair<bool, gen::Chunk::Vector3Type>, 6> checks =
    {{
        { localPos.x == 0, { -1, 0, 0 } },
        { localPos.x == gen::Chunk::size - 1, { 1, 0, 0 } },
        { localPos.y == 0, { 0, -1, 0 } },
        { localPos.y == gen::Chunk::size - 1, { 0, 1, 0 } },
        { localPos.z == 0, { 0, 0, -1 } },
        { localPos.z == gen::Chunk::size - 1, { 0, 0, 1 } }
    }};

    for (const auto& [cond, vec] : checks) {
        if (cond && m_chunks.contains(target + vec)) {
            regenerate(target + vec);
        }
    }
}

gen::Voxel ChunkManager::getVoxelFromLoadedChunks(
    const gen::utils::Vector3<gen::Chunk::ValueType>& worldPos) const
{
    const gen::Chunk::Vector3Type chunkPos{
        static_cast<int>(std::floor(worldPos.x / gen::Chunk::size)),
        static_cast<int>(std::floor(worldPos.y / gen::Chunk::size)),
        static_cast<int>(std::floor(worldPos.z / gen::Chunk::size))
    };

    const auto it = m_chunks.find(chunkPos);
    if (it == m_chunks.end() || !it->second.chunk) {
        return m_generator->getVoxel(worldPos);
    }

    if (const auto localPos = worldPos.as<int32_t>() - chunkPos * gen::Chunk::size;
        gen::Chunk::isValid(localPos)) {
        return it->second.chunk->getVoxel(localPos);
    }
    return m_generator->getVoxel(worldPos);
}

void ChunkManager::generate(const gen::Chunk::Vector3Type position)
{
    using ReturnType = std::pair<gen::Mesh, std::shared_ptr<gen::Chunk>>;

    ChunkData data{ position, ChunkData::State::Generating };
    m_chunks[position] = std::move(data);

    auto* watcher = new QFutureWatcher<ReturnType>(this);
    connect(watcher, &QFutureWatcher<ReturnType>::finished, this, [this, watcher, position]
    {
        if (const auto& [mesh, chunk] = watcher->result(); !chunk->isOnlyAir()) {
            const auto [x, y, z] = position.as<gen::Chunk::ValueType>();
            const auto geometry = std::make_shared<TerrainGeometry>();
            geometry->setMesh(mesh);
            geometry->setChunkPosition({ x, y, z });
            finishGeneration({ position, ChunkData::State::Loaded, geometry, chunk });
        } else {
            finishGeneration({ position, ChunkData::State::Unloaded });
        }
        watcher->deleteLater();
    }, Qt::QueuedConnection);

    const auto future = QtConcurrent::run([this, position]
    {
        const auto chunk = std::make_shared<gen::Chunk>(m_generator->generate(position));
        const auto mesh = gen::MarchingCubes::generateChunk(
            *chunk, [this](const gen::Mesh::Vector3Type pos) {
                return getVoxelFromLoadedChunks(pos);
            });

        return ReturnType{ mesh, chunk };
    });

    watcher->setFuture(future);
}

void ChunkManager::regenerate(const gen::Chunk::Vector3Type position)
{
    const auto it = m_chunks.find(position);
    if (it == m_chunks.end() || !it->second.chunk) {
        return;
    }

    auto chunk = it->second.chunk;

    using ReturnType = gen::Mesh;
    auto* watcher = new QFutureWatcher<ReturnType>(this);

    connect(watcher, &QFutureWatcher<ReturnType>::finished, this, [this, watcher, position]
    {
        const auto& mesh = watcher->result();
        if (const auto chunkIt = m_chunks.find(position);
            chunkIt != m_chunks.end() && chunkIt->second.geometry) {
            // auto chunk =
            // unload this chunk and create the new one
            chunkIt->second.geometry->setMesh(mesh);
        }
        watcher->deleteLater();
    }, Qt::QueuedConnection);

    const auto future = QtConcurrent::run([this, chunk]
    {
        m_generator->regenerateIgnoreAir(*chunk);
        return gen::MarchingCubes::generateChunk(*chunk,
            [this](const gen::Mesh::Vector3Type pos) {
                return getVoxelFromLoadedChunks(pos);
            });
    });

    watcher->setFuture(future);
}

void ChunkManager::finishGeneration(ChunkData&& generatedChunk)
{
    if (const auto it = m_chunks.find(generatedChunk.position);
        it != m_chunks.end()) {
        it->second = std::move(generatedChunk);
        if (it->second.state == ChunkData::State::Loaded) {
            m_generated.push_back(it->second.geometry.get());
        }
        if (--jobsCount == 0 || m_generated.size() >= 4) {
            m_activeChunks.addChunks(m_generated);
            m_generated.clear();
        }
    }
}

void ChunkManager::unload(const gen::Chunk::Vector3Type position)
{
    if (const auto it = m_chunks.find(position);
        it != m_chunks.end()) {
        if (it->second.geometry) {
            m_activeChunks.removeChunk(it->second.geometry.get());
        }
        m_chunks.erase(it);
    }
}

} // app
