# How to Build(mac osx)

### Step 1

- install tools and sdks using [brew](https://brew.sh)

```
brew install cmake folly boost
```

- download other 3rd party sdk:

  - [PhysX SDK](https://developer.nvidia.com/physx-sdk) : You may get source on GitHub and compile by yourself. Current version is 3.3.4.


  - [Autodesk FBX SDK](http://usa.autodesk.com/adsk/servlet/pc/item?siteID=123112&id=26416130): current version is 2018.1.1.

### Step 2

```shell
git clone https://github.com/yushroom/FishEngine.git
cd FishEngine
```

### Step 3

Use CMake to generate project file using any [desired generator](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).

```shell
mkdir -p Engine/build && cd Engine/build
cmake --warn-uninitialized --warn-ununsed-vars -G "Xcode" ..
```

### Step 4

Finally, you can use any IDE according to you selected generator, but you can also just build straight from CMake. Build results can be found in Engine/Binary.

```shell
cmake --build . --target FishEditor --config Release
```



