# aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
SETUP_TEST(AssetSerialization)

target_link_libraries(AssetSerialization yaml-cpp)