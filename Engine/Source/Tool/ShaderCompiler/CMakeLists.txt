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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Runtime)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../ThirdParty/boost_1_61_0/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../ThirdParty/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../ThirdParty/libs/osx/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/PhysXSDK/Include/)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../Runtime ${CMAKE_CURRENT_BINARY_DIR}/FishEngine)

add_executable (ShaderCompiler ${SRCS})
target_compile_options(ShaderCompiler PUBLIC -std=c++14)
target_link_libraries(ShaderCompiler ${Boost_LIBRARIES})
target_link_libraries(ShaderCompiler FishEngine)
target_link_libraries(ShaderCompiler libassimp.a)
target_link_libraries(ShaderCompiler libglfw3.a)
target_link_libraries(ShaderCompiler libGLEW.a)
set_target_properties(ShaderCompiler PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")