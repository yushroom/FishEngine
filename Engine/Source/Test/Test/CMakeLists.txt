# aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
SETUP_TEST(Test)
target_link_libraries(Test yaml-cpp)