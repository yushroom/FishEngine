cmake_minimum_required(VERSION 2.8)
project(FishEngine CXX)
set (FishEngine_VERSION_MAJOR 0)
set (FishEngine_VERSION_MINOR 1)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/Binary)

#boost
set(Boost_USE_STATIC_LIBS       ON) # find static libs
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    ON)
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
include_directories(./ThirdParty/glew)

if (MSVC)
	link_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libs/win32/)
	link_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libs/win32/debug)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
else()
	link_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libs/osx/)
endif()

aux_source_directory(./ThirdParty/imgui imgui_SRCS)

add_library(imgui ${imgui_SRCS})
target_compile_options(imgui PUBLIC -std=c++14)

add_subdirectory(./Source/Runtime)
add_subdirectory(./Source/Editor)
add_subdirectory(./Source/Tool/ShaderCompiler)

#target_link_libraries(FishEditor FishEngine)
