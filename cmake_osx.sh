mkdir -p Engine/build && cd Engine/build
cmake -G "Xcode" -DQt5_DIR=/Users/yushroom/program/library/Qt5.8/5.8/clang_64/lib/cmake/Qt5 -DPhysX_ROOT_DIR=/Users/yushroom/program/github/PhysX-3.3/PhysXSDK -DFBXSDK_DIR="/Users/yushroom/program/library/FBX SDK/2017.1" .. 

