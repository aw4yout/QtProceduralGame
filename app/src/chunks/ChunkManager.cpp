#include "ChunkManager.hpp"
#include "TerrainGeometry.hpp"

#include <world/TerrainGenerator.hpp>
#include <meshing/MarchingCubes.hpp>

namespace app {

using namespace QtTaskTree;

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
            std::abs(position.y - playerChunk.y),
            std::abs(position.z - playerChunk.z)
        });

        if (dist > m_renderDistance + 1) {
            toUnload.push_back(position);
        }
    }

    for (const auto& position : toUnload) {
        unload(position);
    }

    QList<ChunkDataPtrType> renderChunks, borderChunks;
    for (int32_t radius = 0; radius <= m_renderDistance + 1; ++radius) {
        for (int32_t x = -radius; x <= radius; ++x) {
            for (int32_t z = -radius; z <= radius; ++z) {
                for (int32_t y = -radius; y <= radius; ++y) {
                    const auto dist = std::max({ std::abs(x), std::abs(y), std::abs(z) });

                    if (dist != radius) {
                        continue;
                    }

                    const auto target = playerChunk + gen::utils::Vector3{ x, y, z };

                    if (dist <= m_renderDistance && renderChunks.size() < chunksCount) {
                        if (auto it = m_chunks.find(target); it == m_chunks.end()) {
                            auto chunk = std::make_shared<ChunkData>(target);
                            m_chunks[target] = chunk;
                            renderChunks.append(chunk);
                        } else if (it->second->state != ChunkData::State::Meshed) {
                            renderChunks.append(it->second);
                        }
                    } else if (dist == m_renderDistance + 1 || renderChunks.size() >= chunksCount) {
                        if (auto it = m_chunks.find(target); it == m_chunks.end()) {
                            auto chunk = std::make_shared<ChunkData>(target);
                            m_chunks[target] = chunk;
                            borderChunks.append(chunk);
                        } else if (it->second->state == ChunkData::State::Unloaded) {
                            borderChunks.append(it->second);
                        }
                    }
                }
            }
        }
    }

    if (renderChunks.isEmpty() && borderChunks.isEmpty()) {
        return;
    }

    auto* taskTree = new QTaskTree(this);
    taskTree->setRecipe({ recipe(renderChunks, borderChunks) });
    taskTree->start();
    connect(taskTree, &QTaskTree::done, this, [taskTree](const auto) { taskTree->deleteLater(); });
}

void ChunkManager::setVoxel(const QVector3D& voxelPos, const bool solid)
{
    const auto center = gen::utils::Vector3{ voxelPos.x(), voxelPos.y(), voxelPos.z() }.as<int>();

    std::unordered_set<gen::Chunk::Vector3Type, gen::utils::Vector3iHash> chunksToRemesh;

    const auto getChunkPos = [](const gen::Chunk::Vector3Type pos) {
        const auto [x, y, z] = pos.as<float>() / gen::Chunk::size;
        return gen::utils::Vector3{ std::floor(x), std::floor(y), std::floor(z) }.as<int>();
    };

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                const auto pos = center + gen::utils::Vector3{ x, y, z };
                const auto voxelChunk = getChunkPos(pos);

                auto it = m_chunks.find(voxelChunk);
                if (it == m_chunks.end() || !it->second->chunk) {
                    continue;
                }

                const auto localPos = pos - voxelChunk * gen::Chunk::size;
                if (!gen::Chunk::isValid(localPos)) {
                    continue;
                }

                it->second->chunk->getVoxel(localPos) = solid
                    ? gen::Voxel{ 1.0f, gen::Voxel::Material::Snow }
                    : gen::Voxel{ -1.0f, gen::Voxel::Material::Air };
                chunksToRemesh.insert(voxelChunk);

                if (localPos.x == 0) {
                    chunksToRemesh.insert({ voxelChunk.x - 1, voxelChunk.y, voxelChunk.z });
                } else if (localPos.x == gen::Chunk::size - 1) {
                    chunksToRemesh.insert({voxelChunk.x + 1, voxelChunk.y, voxelChunk.z });
                }
                if (localPos.y == 0) {
                    chunksToRemesh.insert({ voxelChunk.x, voxelChunk.y - 1, voxelChunk.z });
                } else if (localPos.y == gen::Chunk::size - 1) {
                    chunksToRemesh.insert({ voxelChunk.x, voxelChunk.y + 1, voxelChunk.z });
                }
                if (localPos.z == 0) {
                    chunksToRemesh.insert({ voxelChunk.x, voxelChunk.y, voxelChunk.z - 1 });
                }
                else if (localPos.z == gen::Chunk::size - 1) {
                    chunksToRemesh.insert({ voxelChunk.x, voxelChunk.y, voxelChunk.z + 1 });
                }
            }
        }
    }

    if (chunksToRemesh.empty()) {
        return;
    }

    QList<ChunkDataPtrType> toRemesh;
    for (const auto& pos : chunksToRemesh) {
        if (auto it = m_chunks.find(pos); it != m_chunks.end()) {
            it->second->chunk->setOnlySolid(false);
            it->second->state = ChunkData::State::Generated;
            toRemesh.append(it->second);
        }
    }

    if (!toRemesh.isEmpty()) {
        auto* taskTree = new QTaskTree(this);
        taskTree->setRecipe({ generateMeshesTask(toRemesh) });
        taskTree->start();
        connect(taskTree, &QTaskTree::done, this, [taskTree](const auto) { taskTree->deleteLater(); });
    }
}

gen::Voxel ChunkManager::getVoxelFromLoadedChunks(
    const gen::utils::Vector3<gen::Chunk::ValueType>& worldPos) const
{
    const gen::utils::Vector3 flooredWorld = {
        std::floor(worldPos.x), std::floor(worldPos.y), std::floor(worldPos.z)
    };
    const gen::Chunk::Vector3Type chunkPos{
        static_cast<int>(std::floor(flooredWorld.x / gen::Chunk::size)),
        static_cast<int>(std::floor(flooredWorld.y / gen::Chunk::size)),
        static_cast<int>(std::floor(flooredWorld.z / gen::Chunk::size))
    };

    const auto it = m_chunks.find(chunkPos);
    if (it == m_chunks.end() || !it->second->chunk) {
        // decided to leave it here for now - there are some rare edge-cases
        // assert(false && "should be unreachable");
        return m_generator->getVoxel(worldPos);
        return {};
    }

    if (const auto localPos = flooredWorld.as<int>() - chunkPos * gen::Chunk::size;
        gen::Chunk::isValid(localPos)) {
        return it->second->chunk->getVoxel(localPos);
    }
    assert(false && "should be unreachable");
    //return m_generator->getVoxel(worldPos);
    return {};
}

void ChunkManager::finishGeneration(ChunkDataPtrType generatedChunk)
{
    if (const auto it = m_chunks.find(generatedChunk->position);
        it != m_chunks.end()) {
        it->second = std::move(generatedChunk);
        if (it->second->geometry->vertexCount() > 0) {
            m_activeChunks.addChunks({ it->second->geometry.get() });
        }
    }
}

void ChunkManager::unload(const gen::Chunk::Vector3Type position)
{
    if (const auto it = m_chunks.find(position);
        it != m_chunks.end()) {
        it->second->cancelled = true;
        if (it->second->geometry) {
            m_activeChunks.removeChunk(it->second->geometry.get());
        }
        m_chunks.erase(it);
    }
}

Group ChunkManager::generateChunksTask(const QList<ChunkDataPtrType>& chunks) const
{
    ListIterator iterator{ chunks };

    const auto groupSetup = [iterator] {
        if (auto& state = (*iterator)->state; state == ChunkData::State::Unloaded) {
            state = ChunkData::State::Generating;
        }
    };
    const auto checkCancelledOrGenerated = [iterator] {
        return (*iterator)->cancelled.load()
            || (*iterator)->state != ChunkData::State::Generating;
    };
    const auto task = [this](const auto& chunk) {
        chunk->chunk = std::make_shared<gen::Chunk>(m_generator->generate(chunk->position));
    };
    const auto groupDone = [iterator] { (*iterator)->state = ChunkData::State::Generated; };

    return For(iterator) >> Do {
        finishAllAndSuccess,
        parallel,
        Group {
            stopOnError,
            onGroupSetup(groupSetup),
            If (checkCancelledOrGenerated) >> Then { errorItem },
            QThreadFunctionTask<void>([task, iterator](auto& thread) {
                thread.setThreadFunctionData(task, *iterator);
            }),
            onGroupDone(groupDone, CallDoneFlag::OnSuccess)
        }
    };
}

Group ChunkManager::generateMeshesTask(const QList<ChunkDataPtrType>& chunks)
{
    using ReturnType = std::pair<ChunkDataPtrType, gen::Mesh>;

    ListIterator iterator{ chunks };
    Storage<std::vector<ReturnType>> storage;

    const auto groupSetup = [iterator] {
        if (auto& state = (*iterator)->state; state == ChunkData::State::Generated) {
            state = ChunkData::State::Meshing;
        }
    };
    const auto checkCancelledOrEmpty = [iterator] {
        return (*iterator)->cancelled.load()
            || (*iterator)->chunk->isOnlyAir()
            || (*iterator)->state != ChunkData::State::Meshing;
    };
    const auto task = [this](const auto& chunk) -> ReturnType {
        auto mesh = gen::MarchingCubes::generateChunk(*chunk->chunk,
            [this](const gen::Mesh::Vector3Type pos) {
                return getVoxelFromLoadedChunks(pos);
            });
        return { chunk, std::move(mesh) };
    };
    const auto groupDone = [iterator] { (*iterator)->state = ChunkData::State::Meshed; };
    const auto forGroupDone = [storage, this] {
        for (auto& [chunk, mesh] : *storage) {
            const auto [x, y, z] = chunk->position.as<gen::Chunk::ValueType>();
            if (!chunk->geometry) {
                const auto geometry = std::make_shared<TerrainGeometry>();
                geometry->setMesh(mesh);
                geometry->setChunkPosition({ x, y, z });
                chunk->geometry = geometry;
                finishGeneration(chunk);
            } else {
                const auto prevCount = chunk->geometry->vertexCount();
                chunk->geometry->setMesh(mesh);
                if (prevCount == 0 && chunk->geometry->vertexCount() > 0) {
                    m_activeChunks.addChunks({ chunk->geometry.get() });
                } else if (prevCount > 0 && chunk->geometry->vertexCount() == 0) {
                    m_activeChunks.removeChunk(chunk->geometry.get());
                }
            }
        }
    };

    return For(iterator) >> Do {
        finishAllAndSuccess,
        parallel,
        storage,
        Group {
            stopOnError,
            onGroupSetup(groupSetup),
            If (checkCancelledOrEmpty) >> Then { errorItem },
            QThreadFunctionTask<ReturnType>(
                [task, iterator](auto& thread) {
                    thread.setThreadFunctionData(task, *iterator);
                },
                [storage](auto& thread) {
                    storage->emplace_back(thread.takeResult());
                }),
            onGroupDone(groupDone, CallDoneFlag::OnSuccess)
        },
        onGroupDone(forGroupDone)
    };
}

ExecutableItem ChunkManager::recipe(
    const QList<ChunkDataPtrType>& renderChunks,
    const QList<ChunkDataPtrType>& borderChunks)
{
    return Group {
        sequential,
        generateChunksTask(renderChunks + borderChunks),
        generateMeshesTask(renderChunks)
    };
}

} // app
