mkdir -p Engine/build && cd Engine/build
export PATH="/usr/local/opt/qt5/bin:$PATH"
cmake --warn-uninitialized --warn-ununsed-vars -G "Xcode" \
    -DPhysX_ROOT_DIR="/Users/yushroom/program/github/PhysX-3.4/PhysX_3.4" \
    -DFBXSDK_DIR="/Users/yushroom/program/library/FBX SDK/2018.1.1" ..

