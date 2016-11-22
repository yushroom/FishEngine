aux_source_directory(. SRCS)
FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_library (imgui STATIC ${SRCS} ${HEADERS})