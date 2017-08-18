### Step 1

- install tools and sdks using [brew](https://brew.sh)

```
brew install cmake qt folly boost
```

- download other 3rd party sdk:

```
(WIP)
```

### Step 2

```shell
git clone https://github.com/yushroom/FishEngine.git
```

### Step 3

Use CMake to generate project file using any [desired generator](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).

```shell
mkdir -p Engine/build && cd Engine/build
export PATH="/usr/local/opt/qt5/bin:$PATH"
cmake --warn-uninitialized --warn-ununsed-vars -G "Xcode" ..
```

### Step 4

Finally, you can use any IDE according to you selected generator, but you can also just build straight from CMake. Build results can be found in Engine/Binary.

```shell
cmake --build . --target FishEditor --config Release
```



