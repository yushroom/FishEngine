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
    filesystem
    wave
    thread
    date_time
    chrono)
include_directories(${Boost_INCLUDE_DIRS})

#message("HHHHHHHHHH" ${Boost_INCLUDE_DIRS})

include_directories(./Source/Runtime)
include_directories(./ThirdParty/boost_1_61_0/)
include_directories(./ThirdParty/)
include_directories(./ThirdParty/PhysXSDK/Include/)
include_directories(./ThirdParty/glew-2.0.0/include)
include_directories(./ThirdParty/glfw-3.2.1/include)
include_directories(./ThirdParty/assimp-3.3.1/include)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
else()
        #link_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libs/osx/)
endif()

add_subdirectory(./ThirdParty/imgui)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(./ThirdParty/glfw-3.2.1/)

set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(./ThirdParty/glew-2.0.0/build/cmake)

set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(./ThirdParty/assimp-3.3.1)

add_subdirectory(./Source/Runtime)
add_subdirectory(./Source/Editor)
add_subdirectory(./Source/Tool/ShaderCompiler)
add_subdirectory(./Source/Test)

#target_link_libraries(FishEditor FishEngine)
