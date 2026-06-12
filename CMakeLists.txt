cmake_minimum_required(VERSION 3.16)

project(ProceduralGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Quick3D Quick3DPhysics)

qt_standard_project_setup(REQUIRES 6.11.1)

qt_add_executable(ProceduralGameApplication
    app/src/main.cpp
    app/src/geometry/TerrainGeometry.cpp
    app/src/geometry/TerrainGeometry.hpp
)

qt_add_qml_module(ProceduralGameApplication
    URI ProceduralGame
    VERSION 1.0
    QML_FILES
    app/qml/Main.qml
    app/qml/graphics/Viewport.qml
)

set_target_properties(ProceduralGameApplication PROPERTIES
    WIN32_EXECUTABLE TRUE
)

add_library(ProceduralGameLib
    core/src/noise/PerlinNoise.cpp
    core/include/noise/PerlinNoise.hpp
    core/src/world/TerrainGenerator.cpp
    core/include/world/TerrainGenerator.hpp
    core/include/utils/Vector.hpp
    core/include/utils/Math.hpp
    core/src/data/Chunk.cpp
    core/include/data/Chunk.hpp
    core/src/data/Mesh.cpp
    core/include/data/Mesh.hpp
    core/include/meshing/MarchingCubes.cpp
    core/include/meshing/MarchingCubes.hpp
)

target_include_directories(ProceduralGameLib
    PUBLIC libraries/ core/include
)

target_link_libraries(ProceduralGameApplication
    PRIVATE Qt6::Quick Qt6::Quick3D Qt6::Quick3DPhysics
    ProceduralGameLib
)

# for QML_ELEMENT to work properly
target_include_directories(ProceduralGameApplication
    PRIVATE app/src/geometry
)
