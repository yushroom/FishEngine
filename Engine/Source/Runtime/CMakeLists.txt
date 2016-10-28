aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
add_library (FishEngine ${SRCS})
target_compile_options(FishEngine PUBLIC -std=c++14)