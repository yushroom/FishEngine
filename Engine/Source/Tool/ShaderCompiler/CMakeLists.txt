aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
add_executable (ShaderCompiler ${SRCS})
target_link_libraries(ShaderCompiler FishEngine)
