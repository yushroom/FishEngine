cmake_minimum_required (VERSION 2.8)
project (FishEngine CXX)
aux_source_directory(../source FishEngine_SRCS)
aux_source_directory(../source/FishEditor FishEditor_SRCS)
aux_source_directory(../external/imgui imgui_SRCS)

# boost
#set(Boost_USE_STATIC_LIBS       ON) # find static libs
#set(Boost_USE_MULTITHREADED     ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED
    COMPONENTS
    system
    filesystem
    serialization)
include_directories($(Boost_INCLUDE_DIRS))
#target_link_libraries()
include_directories(../external/)
include_directories(../external/PhysXSDK/Include/)

add_library(imgui ${imgui_SRCS})
target_compile_options(imgui PUBLIC -std=c++14)

add_library(FishEngine ${FishEngine_SRCS})
target_compile_options(FishEngine PUBLIC -std=c++14)

include_directories(../source)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/libs/osx)
add_executable(FishEditor ${FishEditor_SRCS})
target_link_libraries(FishEditor FishEngine imgui)
find_package(zlib)
target_link_libraries(FishEditor ${ZLIB_LIBRARIES})
target_link_libraries(FishEditor libassimp.a libfbxsdk.a libglfw3.a libglew.a)
FILE(GLOB PyhsXLibs "../external/libs/osx/lib*.a")
target_link_libraries(FishEditor ${PyhsXLibs})
target_link_libraries(FishEditor ${Boost_LIBRARIES})
target_compile_options(FishEditor PUBLIC -std=c++14)
set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")