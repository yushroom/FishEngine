
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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

SOURCE_GROUP( Asset FILES ${Asset_SRCS} )
SOURCE_GROUP( UI    FILES ${UI_SRCS}    )
SOURCE_GROUP( MOC   FILES ${MOC_SRCS}   )


SET( ASSETS_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../assets)
SET( SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Shaders)
IF( NOT WIN32 )
    FILE( GLOB Models ${ASSETS_DIR}/Models/*.txt)
    FILE( GLOB Gizmos ${ASSETS_DIR}/Textures/Gizmos/*.png)
    FILE( GLOB Shader_headers ${SHADER_DIR}/include/*.inc)
    FILE( GLOB Editor_shaders ${SHADER_DIR}/Editor/*.*)
    FILE( GLOB Shaders ${SHADER_DIR}/*.*)
    SET( RESOURCE_FILES ${Models} ${Gizmos} ${Shaders} ${Shader_headers} ${Editor_shaders})
ENDIF()

add_executable(FishEditor MACOSX_BUNDLE ${SRCS} ${UI_SRCS} ${FORMS} ${ReflectFilesSources} resources.qrc ${RESOURCE_FILES})

IF( NOT WIN32 )
    SET_SOURCE_FILES_PROPERTIES(${Models} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Models)
    SET_SOURCE_FILES_PROPERTIES(${Gizmos} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Textures/Gizmos)
    SET_SOURCE_FILES_PROPERTIES(${Shaders} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders)
    SET_SOURCE_FILES_PROPERTIES(${Shader_headers} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders/include)
    SET_SOURCE_FILES_PROPERTIES(${Editor_shaders} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders/Editor)
ENDIF()

IF( WIN32 )
    target_link_libraries(FishEditor filesystem)
    target_link_libraries(FishEditor system)
ENDIF()
target_link_libraries(FishEditor FishEngine)
target_link_libraries(FishEditor assimp)
target_link_libraries(FishEditor yaml-cpp)
target_link_libraries(FishEditor ${PhysXSDK_LIBRARIES})
target_link_libraries(FishEditor ${Boost_LIBRARIES})

if (MSVC)
    target_link_libraries(FishEditor ${FBXSDK_LIB})
    target_link_libraries(FishEditor opengl32.lib)
    target_link_libraries(FishEditor glew_s)
else()
    set_target_properties(FishEditor PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
endif()

target_link_libraries(FishEditor Qt5::Widgets)


#https://gist.github.com/Rod-Persky/e6b93e9ee31f9516261b

# find the release *.dll file
get_target_property(Qt5_CoreLocation Qt5::Core LOCATION)
get_target_property(Qt5_GuiLocation Qt5::Gui LOCATION)
get_target_property(Qt5_WidgetsLocation Qt5::Widgets LOCATION)

# find the debug *d.dll file
get_target_property(Qt5_CoreLocationDebug Qt5::Core LOCATION)
get_target_property(Qt5_GuiLocationDebug Qt5::Gui LOCATION)
get_target_property(Qt5_WidgetsLocationDebug Qt5::Widgets LOCATION)

# message(${Qt5_CoreLocation})
# message(${Qt5_GuiLocation})
# message(${Qt5_WidgetsLocation})

if( WIN32 )
    macro(qt5_copy_dll APP DLL)
        # find the release *.dll file
        get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
        # find the debug *d.dll file
        get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)

        add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}> $<TARGET_FILE_DIR:${APP}>)
    endmacro()

    qt5_copy_dll(FishEditor Core)
    qt5_copy_dll(FishEditor Gui)
    qt5_copy_dll(FishEditor Widgets)

    set(PhysX_BIN_DIR ${PhysX_ROOT_DIR}/Bin/vc14win64)
    message(${PhysX_BIN_DIR}/nvToolsExt64_1.dll)
    message(${PhysX_BIN_DIR}/PhysX3CommonDEBUG_x64.dll)
    message(${PhysX_BIN_DIR}/PhysX3DEBUG_x64.dll)

    add_custom_command(TARGET FishEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PhysX_BIN_DIR}/nvToolsExt64_1.dll $<TARGET_FILE_DIR:FishEditor>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${PhysX_BIN_DIR}/PhysX3CommonDEBUG_x64.dll> $<$<NOT:$<CONFIG:Debug>>:${PhysX_BIN_DIR}/PhysX3CommonCHECKED_x64.dll> $<TARGET_FILE_DIR:FishEditor>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${PhysX_BIN_DIR}/PhysX3DEBUG_x64.dll> $<$<NOT:$<CONFIG:Debug>>:${PhysX_BIN_DIR}/PhysX3CHECKED_x64.dll> $<TARGET_FILE_DIR:FishEditor>
    )

    add_custom_command(TARGET FishEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:FishEditor>/Resources
    )

    add_custom_command(TARGET FishEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} $<TARGET_FILE_DIR:FishEditor>/Shaders
    )
endif()
