cmake_minimum_required (VERSION 2.8)
project (FishEngine CXX)
aux_source_directory(. SRCS)

# boost
find_package(Boost 1.59 REQUIRED
    COMPONENTS
    system
    filesystem
    serialization)
include_directories($(Boost_INCLUDE_DIRS))
include_directories(../external/)
include_directories(../external/PhysXSDK/Include/)

add_library (FishEngine ${SRCS})
target_compile_options(FishEngine PUBLIC -std=c++14)