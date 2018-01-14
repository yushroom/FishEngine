SET(FishGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../FishGUI)
SET(FishGUI_INCLUDE_DIR ${FishGUI_DIR}/include)
SET(FishGUI_SRC_DIR ${FishGUI_DIR}/src)

aux_source_directory(${FishGUI_SRC_DIR} SRCS)
FILE(GLOB_RECURSE HEADERS ${FishGUI_SRC_DIR}/../../Include/FishGUI/*.hpp)
# FILE(GLOB_RECURSE HEADERS2 ${FishGUI_SRC_DIR}/*.h ${FishGUI_SRC_DIR}/*.hpp)

include_directories( ${FishGUI_INCLUDE_DIR} )
# include_directories(${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/nanovg)

aux_source_directory( ${FishGUI_SRC_DIR} SRCS )
FILE(GLOB_RECURSE HEADERS ${FishGUI_INCLUDE_DIR}/*.hpp ${FishGUI_INCLUDE_DIR}/*.h)

# nanovg
set(NANO_VG_ROOT_DIR ${FishGUI_DIR}/ThirdParty/nanovg)
FILE(GLOB_RECURSE NANO_VG_SRCS ${NANO_VG_ROOT_DIR}/*.c)
FILE(GLOB_RECURSE NANO_VG_HEADERS ${NANO_VG_ROOT_DIR}/*.h)
# message('hhhhh', ${NANO_VG_ROOT_DIR})
source_group("nanovg" FILES ${NANO_VG_SRCS} ${NANO_VG_HEADERS})
include_directories( ${NANO_VG_ROOT_DIR} )

add_library(FishGUI STATIC ${SRCS} ${HEADERS} ${NANO_VG_SRCS} ${NANO_VG_HEADERS})

# source_group("nanovg" FILES ${NANO_VG_SRCS} ${NANO_VG_HEADERS})
# target_link_libraries(FishGUI glfw)

# FILE(GLOB_RECURSE NANO_VG_SRCS ./ThirdParty/nanovg/*.c)
# FILE(GLOB_RECURSE NANO_VG_HEADERS ./ThirdParty/nanovg/*.h)

# add_executable(Demo ${SRCS} ${HEADERS} ${NANO_VG_SRCS} ${NANO_VG_HEADERS})

# if (MSVC)
#     target_link_libraries(FishGUI opengl32.lib)
#     target_link_libraries(FishGUI glew)
# else()
#     set_target_properties(FishGUI PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
# endif()