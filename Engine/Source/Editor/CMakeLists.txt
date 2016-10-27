cmake_minimum_required(VERSION 2.8)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

#boost
set(Boost_USE_STATIC_LIBS       ON) # find static libs
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED
    COMPONENTS
    system
    filesystem)
include_directories($(Boost_INCLUDE_DIRS))

FILE(GLOB PhysXSDK_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/libs/osx/PhysXSDK/*.a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Runtime)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/boost_1_61_0/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/PhysXSDK/Include/)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/libs/osx/)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Runtime ${CMAKE_CURRENT_BINARY_DIR}/FishEngine)

add_executable (FishEditor ${SRCS})
target_compile_options(FishEditor PUBLIC -std=c++14)
target_link_libraries(FishEditor ${Boost_LIBRARIES})
target_link_libraries(FishEditor ${PhysXSDK_LIBRARIES})
target_link_libraries(FishEditor FishEngine)
target_link_libraries(FishEditor imgui)
target_link_libraries(FishEditor libassimp.a)
target_link_libraries(FishEditor libglfw3.a)
target_link_libraries(FishEditor libGLEW.a)
set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")