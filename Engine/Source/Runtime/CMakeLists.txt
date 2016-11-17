aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.hpp)
add_library (FishEngine ${SRCS} ${HEADERS})
target_compile_options(FishEngine PUBLIC -std=c++14)
