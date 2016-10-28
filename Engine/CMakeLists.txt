cmake_minimum_required(VERSION 2.8)
project(FishEngine CXX)

#boost
set(Boost_USE_STATIC_LIBS       ON) # find static libs
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED
    COMPONENTS
    system
    filesystem)
include_directories(${Boost_INCLUDE_DIRS})

message("HHHHHHHHHH" ${Boost_INCLUDE_DIRS})

include_directories(./Source/Runtime)
include_directories(./ThirdParty/boost_1_61_0/)
include_directories(./ThirdParty/)
include_directories(./ThirdParty/PhysXSDK/Include/)


link_directories(./ThirdParty/libs/osx/)


aux_source_directory(./ThirdParty/imgui imgui_SRCS)

add_library(imgui ${imgui_SRCS})
target_compile_options(imgui PUBLIC -std=c++14)

add_subdirectory(./Source/Runtime)
add_subdirectory(./Source/Editor)
add_subdirectory(./Source/Tool/ShaderCompiler)

#target_link_libraries(FishEditor FishEngine)
