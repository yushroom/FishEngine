if not exist build md build
cd build
cmake -G "Visual Studio 14 Win64" ..
cmake --build . --config RelWithDebInfo
pause