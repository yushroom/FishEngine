cmake_minimum_required(VERSION 2.8)
project(FishEngine CXX)

aux_source_directory(./ThirdParty/imgui imgui_SRCS)

add_library(imgui ${imgui_SRCS})
target_compile_options(imgui PUBLIC -std=c++14)

add_subdirectory(./Source/Runtime)
add_subdirectory(./Source/Editor)
add_subdirectory(./Source/Tool/ShaderCompiler)

#target_link_libraries(FishEditor FishEngine)
