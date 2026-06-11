cmake_minimum_required(VERSION 3.16)

project(ProceduralGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Quick3D Quick3DPhysics)

qt_standard_project_setup(REQUIRES 6.11.1)

qt_add_executable(ProceduralGameApplication
    main.cpp
)

qt_add_qml_module(ProceduralGameApplication
    URI ProceduralGame
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/graphics/Viewport.qml
)

set_target_properties(ProceduralGameApplication PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(ProceduralGameApplication
    PRIVATE Qt6::Quick Qt6::Quick3D Qt6::Quick3DPhysics
)

include(GNUInstallDirs)
install(TARGETS ProceduralGameApplication
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
