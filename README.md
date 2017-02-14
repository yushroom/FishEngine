## What is FishEngine

FishEngine is a simple, Unity-like game engine, mainly written in C++11 (with some C++14 features).



## Features

- Unity-like Editor
- Nearly same API with Unity
- Physically Based Rendering (PBR)
- Asset management (WIP)
- Physics system (WIP)


## Release



## How to Build

1. install Qt5, Boost and PhysX
   Or you can download from here[TBA]

   **for all platforms:**
   1.1 Download and install Qt5 SDK from https://www.qt.io. My choice is version [5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-mac-x64-clang-5.8.0.dmg).

   1.2 Download and install PhysX SDK from https://developer.nvidia.com/physx-sdk.
   You may get source on GitHub and compile by yourself. Current version is 3.3.

   **for windows:**
   1.3 download and install Clang ( http://releases.llvm.org/download.html -> Clang for Windows (64-bit))



2. install [CMake](https://cmake.org/download/) (version 3.0.0 or later is required)

3. ​

   ```shell
   git clone https://github.com/yushroom/FishEngine.git
   ```

4. Use CMake to generate project file
   If you have set path to boost and Qt:

   ```shell
   cd FishEngine/Engine
   mkdir build && cd build
   cmake -G "Visual Studio 14 Win64" ..
   ```

   Or you can use camke-gui.




## 3rd Party Libraries

**for all platforms:**

- [boost](http://www.boost.org/) (filesystem, system)
- [assimp](https://github.com/assimp/assimp) (3D model importer)
- [Qt](https://www.qt.io) (Editor only,  GUI / image loader)
- [gli](https://github.com/g-truc/gli) (dds texture loader) and [glm](https://github.com/g-truc/glm) (required by gli)
- [stb](https://github.com/nothings/stb) (font/image)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) (serialization)
- [PhysX](https://developer.nvidia.com/physx-sdk) (physics system)


**for MS Windows:**

- [clang](http://www.boost.org/) (c++ reflection)
- [glew](https://github.com/nigels-com/glew) (opengl loader)

