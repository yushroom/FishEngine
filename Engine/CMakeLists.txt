CMAKE_MINIMUM_REQUIRED(VERSION 3.6.0 FATAL_ERROR)

PROJECT(FishEngine CXX)

SET(FISHENGINE_VERSION_MAJOR 0)
SET(FISHENGINE_VERSION_MINOR 1)
SET(FISHENGINE_VERSION_PATCH 0)
SET(FISHENGINE_VERSION ${FISHENGINE_VERSION_MAJOR}.${FISHENGINE_VERSION_MINOR}.${FISHENGINE_VERSION_PATCH})

# Global compile & linker flags

# Target at least C++14
set(CMAKE_CXX_STANDARD 14)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Output
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/Binary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(FISHENGINE_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR}/../Script)

#boost
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
MESSAGE("Found Boost: " ${Boost_INCLUDE_DIRS})

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# fmod
SET( FMOD_DIR ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/fmod)
include_directories( ${FMOD_DIR}/include )
IF (WIN32)
	SET( FMOD_LIB_DEBUG ${FMOD_DIR}/lib/fmodL64_vc.lib)
	SET( FMOD_LIB_RELEASE ${FMOD_DIR}/lib/fmod64_vc.lib)
ELSE()
	SET( FMOD_LIB_DEBUG ${FMOD_DIR}/lib/libfmodL.dylib)
	SET( FMOD_LIB_RELEASE ${FMOD_DIR}/lib/libfmod.dylib)
ENDIF()
SET( FMOD_LIB debug ${FMOD_LIB_DEBUG} optimized ${FMOD_LIB_RELEASE} )


include_directories(./Include)
include_directories(./ThirdParty/)

# glfw
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(./ThirdParty/glfw/)
include_directories(./ThirdParty/glfw/include)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "ThirdParty")

if (MSVC)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251" )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
	include_directories(./ThirdParty/glew/include)
	#set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
	ADD_SUBDIRECTORY(./ThirdParty/glew/build/cmake)

	# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
	SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "ThirdParty/glew")
	SET_TARGET_PROPERTIES(glew_s PROPERTIES FOLDER "ThirdParty/glew")
endif()

include_directories(./ThirdParty/yaml-cpp/include)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
#set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(./ThirdParty/yaml-cpp)
SET_TARGET_PROPERTIES(yaml-cpp PROPERTIES FOLDER "ThirdParty/yaml-cpp")
SET_TARGET_PROPERTIES(format PROPERTIES FOLDER "ThirdParty/yaml-cpp")


# SET( PhysX_ROOT_DIR "" CACHE PATH "PhysXSDK root directory" )
SET( PhysX_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/PhysX/PhysX_3.4" )
include_directories( ${PhysX_ROOT_DIR}/Include )
include_directories( ${PhysX_ROOT_DIR}/../PxShared/include )
include_directories( ${PhysX_ROOT_DIR}/Snippets )

if (MSVC)
	# PhysXSDK
	# set(PhysX_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/PhysXSDK)
	set(PhysXSDK_LIBRARIES_DIR ${PhysX_ROOT_DIR}/Lib/vc14win64)
	set(PhysXSDK_LIBRARIES "")
	foreach(x PhysX3 PhysX3Common PhysX3Cooking)
		set(lib1 ${PhysXSDK_LIBRARIES_DIR}/${x}DEBUG_x64.lib)
		set(lib2 ${PhysXSDK_LIBRARIES_DIR}/${x}CHECKED_x64.lib)
		IF(NOT EXISTS ${lib1})
			message(FATAL_ERROR "physx root directory does not exist: ${lib1}")
		endif ()
		IF(NOT EXISTS ${lib2})
			message(FATAL_ERROR "physx root directory does not exist: ${lib2}")
		endif ()
		list (APPEND PhysXSDK_LIBRARIES debug ${lib1} optimized ${lib2})
	endforeach()
	foreach(x PhysX3Extensions PhysX3Vehicle PhysXProfileSDK PhysXVisualDebuggerSDK PxTask)
		set(lib1 ${PhysXSDK_LIBRARIES_DIR}/${x}DEBUG.lib)
		set(lib2 ${PhysXSDK_LIBRARIES_DIR}/${x}CHECKED.lib)
		IF(NOT EXISTS ${lib1})
			message(FATAL_ERROR "physx root directory does not exist: ${lib1}")
		endif ()
		IF(NOT EXISTS ${lib2})
			message(FATAL_ERROR "physx root directory does not exist: ${lib2}")
		endif ()
		list (APPEND PhysXSDK_LIBRARIES debug ${lib1} optimized ${lib2})
	endforeach()
else()
	# FILE(GLOB lib1 ${PhysX_ROOT_DIR}/Lib/osx64/*)
	FILE(GLOB PhysXSDK_LIBRARIES ${PhysX_ROOT_DIR}/Lib/osx64/*.a ${PhysX_ROOT_DIR}/../PxShared/lib/osx64/*.a)
endif()

add_definitions(-DFishEngine_SHARED_LIB=1)

ADD_SUBDIRECTORY(./CMake/FishEngine)
ADD_SUBDIRECTORY(./CMake/FishGame)
ADD_SUBDIRECTORY(./CMake/FishEditor)
ADD_SUBDIRECTORY(./Source/Tool)
ADD_SUBDIRECTORY(./Source/Test)