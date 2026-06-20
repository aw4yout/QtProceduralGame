# QtProceduralGame

This project is a basic minecraft-like (not in a graphical way though!) procedurally generated game.  
For now it's less a game than a showcase of Qt's gamedev capabilities.  
This is also the first project where I implemented parallel execution using [QtTaskTree](https://doc.qt.io/qt-6/qttasktree-index.html).

## Features

- Marching cubes algorithm
- Multi-layer 2D/3D Perlin noise generation
- Destructive terrain
- Biome system
- QtQuick3D's PBR
- Modern C++23/Qt6 features and techniques

## Showcase

... I'll upload screenshots here ...

## Controls

Don't pretend like you don't already know 🫩
- WASD - walk around
- Space - jump
- Mouse - look around
- LMB - break stuff
- Alt+F4 - exit the game

## Build

First of all, make sure you've installed Qt 6.12.0 (beta1 at the time of writing)  

```bash
mkdir build && cd build
# you can also put -DCMAKE_PREFIX_PATH=/your/Qt/installation/6.12.0/compiler_64 instead of Qt6_DIR
cmake .. -DCMAKE_BUILD_TYPE=Release -DQt6_DIR=/your/Qt/installation/6.12.0/compiler_64/lib/cmake/Qt6/
cmake --build . --parallel
./ProceduralGameApplication
```
