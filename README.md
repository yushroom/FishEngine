# FishEngine

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

Before yout build, you will need to have some additional software installed:

- [Git](http://git-scm.com/downloads).
- [CMake](https://cmake.org/download/). Version 3.0.0 or later is required.
- [Python](https://www.python.org/downloads/). Version 2.7.x is required, 3.x is not officially supported because libclang has no python3 binding.
- [Visual Studio 2015](https://www.visualstudio.com/downloads), Update 3 (Windows only)

And some other 3rd-party libs is required:

1. install Qt5, and PhysX
   Or you can download from here[TBA]

   **for all platforms:**
   1.1 Download and install Qt5 SDK from https://www.qt.io. My choice is version [5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-mac-x64-clang-5.8.0.dmg).

   1.2 Download and install PhysX SDK from https://developer.nvidia.com/physx-sdk. You may get source on GitHub and compile by yourself. Current version is 3.3.4.

   **for windows:**
   1.3 download and install [Clang](http://releases.llvm.org/3.9.1/LLVM-3.9.1-win64.exe)



2. ​

   ```shell
   git clone https://github.com/yushroom/FishEngine.git
   ```

3. Use CMake to generate project file.
   If you have Qt in your system path:

   ```shell
   cd FishEngine/Engine
   mkdir build && cd build
   cmake -G "Visual Studio 14 Win64" ..
   ```

   Otherwise, you can use camke-gui to do the same thing.




## 3rd Party Libraries

**for all platforms:**

- [boost](http://www.boost.org/) (filesystem)
- [assimp](https://github.com/assimp/assimp) (3D model importer)
- [Qt](https://www.qt.io) (Editor only,  GUI / image loader)
- [gli](https://github.com/g-truc/gli) (dds texture loader) and [glm](https://github.com/g-truc/glm) (required by gli)
- [stb](https://github.com/nothings/stb) (font/image)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) (serialization)
- [PhysX](https://developer.nvidia.com/physx-sdk) (physics system)


**for MS Windows:**

- [clang](http://www.boost.org/) (c++ reflection)
- [glew](https://github.com/nigels-com/glew) (opengl loader)

