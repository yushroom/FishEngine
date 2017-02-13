# FishEngine

## What is FishEngine

FishEngine is a simple, Unity-like game engine, mainly written in C++11 (with some C++14 features).



## Features

- Unity-like Editor
- Nearly same API with Unity
- Physically based Rendering (PBR)
- Asset management (WIP)
- Physics system (WIP)




## How to build?

1. install Qt5 and boost

   Or you can download from here[TBA]

2. install CMake

3. ​

```shell
git clone https://github.com/yushroom/FishEngine.git
```

4. using CMake to generate project file

   If you have set path to boost and Qt:

   ```shell
   cd FishEngine/Engine
   mkdir build && cd build
   cmake -G "Visual Studio 14 Win64" ..
   ```

   Or you can use camke-gui.

   ​



## 3rd Party libraries

for all platforms

- clang (c++ reflection)

- boost (filesystem, system)

- assimp (3D model importer)

- Qt (Editor only,  GUI / image loader)

- gli (dds texture loader)

- stb (font/image)

- yaml-cpp (serialization)


for MS Windows:

- glew (opengl loader)

