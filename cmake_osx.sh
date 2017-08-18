mkdir -p Engine/build && cd Engine/build
export PATH="/usr/local/opt/qt5/bin:$PATH"
cmake --warn-uninitialized --warn-ununsed-vars -G "Xcode" ..