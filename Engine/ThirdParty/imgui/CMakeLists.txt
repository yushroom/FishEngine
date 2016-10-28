aux_source_directory(. SRCS)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Runtime)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/boost_1_61_0/)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/PhysXSDK/Include/)

add_executable (imgui ${SRCS})
target_compile_options(imgui PUBLIC -std=c++14)