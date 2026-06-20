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

<img width="1280" height="720" src="https://github.com/user-attachments/assets/4802dabf-4551-4f7f-9315-d6e19ef74772" />
<img width="1280" height="720" src="https://github.com/user-attachments/assets/b5401b70-ab39-4f5c-b296-8942c9c4585b" />
<img width="1280" height="720" src="https://github.com/user-attachments/assets/ee0fbd28-19b4-4ca7-942b-d4a070f84838" />

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
