aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

add_executable (ShaderCompiler ${SRCS})
target_compile_options(ShaderCompiler PUBLIC -std=c++14)
target_link_libraries(ShaderCompiler ${Boost_LIBRARIES})
target_link_libraries(ShaderCompiler FishEngine)
target_link_libraries(ShaderCompiler libglfw3.a)
target_link_libraries(ShaderCompiler libGLEW.a)
set_target_properties(ShaderCompiler PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")