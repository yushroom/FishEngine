# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SRCS)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/UI UI_SRCS)
#FILE(GLOB Heasers ${CMAKE_CURRENT_LIST_DIR}/UI/*.hpp)
FILE(GLOB FORMS ${CMAKE_CURRENT_LIST_DIR}/UI/*.ui)
SOURCE_GROUP(Forms FILES ${FORMS})


# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5 COMPONENTS Widgets REQUIRED)

#FILE(GLOB ReflectHeaders ${CMAKE_CURRENT_LIST_DIR}/generate/*.hpp)
FILE(GLOB ReflectFilesSources ${CMAKE_CURRENT_LIST_DIR}/generate/*.cpp)
SOURCE_GROUP(Reflect FILES ${ReflectFilesSources})

foreach (x AssetImporter TextureImporter ModelImporter)
    foreach (ext hpp cpp)
        set(f ${CMAKE_CURRENT_LIST_DIR}/${x}.${ext})
        SET(Asset_SRCS ${Asset_SRCS} ${f})
        IF(NOT EXISTS ${f})
            MESSAGE(FATAL_ERROR "source files does not exist: ${f}")
        ENDIF ()
    endforeach ()
endforeach()
SOURCE_GROUP(Asset FILES ${Asset_SRCS})

SOURCE_GROUP( UI FILES ${UI_SRCS} )
SOURCE_GROUP( MOC FILES ${MOC_SRCS} )
add_executable(FishEditor MACOSX_BUNDLE ${SRCS} ${UI_SRCS} ${FORMS} ${ReflectFilesSources})

include_directories(../build-Editor-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/)
target_link_libraries(FishEditor FishEngine)
target_link_libraries(FishEditor assimp)
target_link_libraries(FishEditor yaml-cpp)
target_link_libraries(FishEditor ${PhysXSDK_LIBRARIES})
target_link_libraries(FishEditor ${Boost_LIBRARIES})

if (MSVC)
    target_link_libraries(FishEditor opengl32.lib)
    target_link_libraries(FishEditor glew_s)
else()
    #set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    #set(CMAKE_CXX_FLAGS "-I/Users/yushroom/program/library/Qt5.8/5.8/clang_64/lib/QtWidgets.framework/Headers -I/Users/yushroom/program/library/Qt5.8/5.8/clang_64/lib/QtGui.framework/Headers -I/Users/yushroom/program/library/Qt5.8/5.8/clang_64/lib/QtCore.framework/Headers -I/Users/yushroom/program/library/Qt5.8/5.8/clang_64/mkspecs/macx-clang")
    #set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-F/Users/yushroom/program/library/Qt5.8/5.8/clang_64/lib -lz -framework QtWidgets -framework QtGui -framework QtCore -framework DiskArbitration -framework IOKit -framework OpenGL -framework AGL")
    set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
endif()

target_link_libraries(FishEditor Qt5::Widgets)