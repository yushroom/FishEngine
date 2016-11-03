aux_source_directory(. SRCS)
FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library (imgui ${SRCS} ${HEADERS})
target_compile_options(imgui PUBLIC -std=c++14)