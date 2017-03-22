cmake_minimum_required(VERSION 3.0)

# Target at least C++14
set(CMAKE_CXX_STANDARD 14)

aux_source_directory( ${CMAKE_CURRENT_LIST_DIR} SRCS )
FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/Assets/*.hpp)

SET(FishEngineRootDir ${CMAKE_CURRENT_LIST_DIR}/../../Engine)
IF (MSVC)
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${FishEngineRootDir}/ThirdParty/boost/)
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${FishEngineRootDir}/ThirdParty/boost/lib64-msvc-14.0)
ENDIF()

set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/Assets)
INCLUDE_DIRECTORIES(${FishEngineRootDir}/Source/Runtime)
LINK_DIRECTORIES(${FishEngineRootDir}/Binary/lib/Debug)

add_library (Sponza SHARED ${SRCS} ${HEADERS} ${Serialization_SRCS} ${SceneManagement_SRCS} ${PrivateFiles})
target_link_libraries(Sponza FishEngine)
