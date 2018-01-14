SET(FishGUI_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Source/FishGUI)

aux_source_directory(${FishGUI_SRC_DIR} SRCS)
FILE(GLOB_RECURSE HEADERS ${FishGUI_SRC_DIR}/../../Include/FishGUI/*.hpp)
FILE(GLOB_RECURSE HEADERS2 ${FishGUI_SRC_DIR}/*.h ${FishGUI_SRC_DIR}/*.hpp)

include_directories( ${FishGUI_SRC_DIR} )
# include_directories(${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/nanovg)

add_library(FishGUI SHARED ${SRCS} ${HEADERS} ${HEADERS2})
target_link_libraries(FishGUI glfw)

# FILE(GLOB_RECURSE NANO_VG_SRCS ./ThirdParty/nanovg/*.c)
# FILE(GLOB_RECURSE NANO_VG_HEADERS ./ThirdParty/nanovg/*.h)

# add_executable(Demo ${SRCS} ${HEADERS} ${NANO_VG_SRCS} ${NANO_VG_HEADERS})

if (MSVC)
    target_link_libraries(FishGUI opengl32.lib)
    target_link_libraries(FishGUI glew)
else()
    set_target_properties(FishGUI PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
endif()