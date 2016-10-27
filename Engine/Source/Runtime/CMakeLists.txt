aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
# boost
# find_package(Boost 1.59 REQUIRED
#     COMPONENTS
#     system
#     filesystem)
# include_directories($(Boost_INCLUDE_DIRS))
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/boost_1_61_0/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/PhysXSDK/Include/)

add_library (FishEngine ${SRCS})
target_compile_options(FishEngine PUBLIC -std=c++14)