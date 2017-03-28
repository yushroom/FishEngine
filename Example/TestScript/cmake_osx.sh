mkdir -p build
cd build
cmake -G "Xcode" ..
cmake --build . --config RelWithDebInfo