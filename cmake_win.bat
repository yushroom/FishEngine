cd Engine
if not exist build md build
cd build
cmake -G "Visual Studio 14 Win64" -DQt5_DIR=D:\Library\Qt\Qt5.8.0\5.8\msvc2015_64\lib\cmake\Qt5 -DPhysX_ROOT_DIR=D:\program\PhysX-3.3\PhysXSDK -DFBXSDK_DIR="D:\Library\FBX SDK\2017.1" ..
pause