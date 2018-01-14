set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

SET(FishEditor_qt_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Source/FishEditor_qt)
include_directories( ${FishEditor_qt_SRC_DIR} )

# Autodesk FBX sdk
# SET( FBXSDK_DIR "" CACHE PATH "Autodesk FBX SDK root directory" )
SET( FBXSDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/FBX SDK/2018.1.1")
include_directories( ${FBXSDK_DIR}/include )
IF (WIN32)
    SET( FBXSDK_LIB_DIR ${FBXSDK_DIR}/lib/vs2015/x64 )
    SET( FBXSDK_LIB_DEBUG ${FBXSDK_LIB_DIR}/debug/libfbxsdk-md.lib )
    SET( FBXSDK_LIB_RELEASE ${FBXSDK_LIB_DIR}/release/libfbxsdk-md.lib )
ELSE()
    SET( FBXSDK_LIB_DIR ${FBXSDK_DIR}/lib/clang )
    SET( FBXSDK_LIB_DEBUG ${FBXSDK_LIB_DIR}/debug/libfbxsdk.a )
    SET( FBXSDK_LIB_RELEASE ${FBXSDK_LIB_DIR}/release/libfbxsdk.a )
ENDIF()

# IF(NOT EXISTS ${FBXSDK_LIB_DEBUG})
#     MESSAGE(FATAL_ERROR "${FBXSDK_LIB_DEBUG} not found")
# ENDIF ()

# IF(NOT EXISTS ${FBXSDK_LIB_RELEASE})
#     MESSAGE(FATAL_ERROR "${FBXSDK_LIB_RELEASE} not found")
# ENDIF ()
# SET( FBXSDK_LIB debug ${FBXSDK_LIB_DEBUG} optimized ${FBXSDK_LIB_RELEASE} )
SET( FBXSDK_LIB ${FBXSDK_LIB_RELEASE} )


# FreeImage
SET(FreeImage_Root ${FishEditor_qt_SRC_DIR}/../../ThirdParty/freeimage)
include_directories(${FreeImage_Root}/include)
link_directories(${FreeImage_Root}/lib)


# Python3
SET( PYTHON3_DIR ${FishEditor_qt_SRC_DIR}/../../ThirdParty/python3 )
include_directories( ${PYTHON3_DIR}/include )
IF (WIN32)
    SET( PYTHON3_LIB debug ${PYTHON3_DIR}/libs/python36.lib optimized ${PYTHON3_DIR}/libs/python36.lib )
ELSE()
    SET( PYTHON3_LIB debug ${PYTHON3_DIR}/libs/libpython3.6dm.a optimized ${PYTHON3_DIR}/libs/libpython3.6m.a )
ENDIF()


# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(${FishEditor_qt_SRC_DIR} SRCS)
FILE(GLOB Heasers ${FishEditor_qt_SRC_DIR}/*.hpp)
aux_source_directory(${FishEditor_qt_SRC_DIR}/UI UI_SRCS)
FILE(GLOB UI_Heasers ${FishEditor_qt_SRC_DIR}/UI/*.hpp)
FILE(GLOB FORMS ${FishEditor_qt_SRC_DIR}/UI/*.ui)
SOURCE_GROUP(Forms FILES ${FORMS})
FILE(GLOB InspectorFiles
    ${FishEditor_qt_SRC_DIR}/Inspector.*
    ${FishEditor_qt_SRC_DIR}/UI/InspectorWidget.*
    ${FishEditor_qt_SRC_DIR}/UI/TextureImporterInspector.*
    ${FishEditor_qt_SRC_DIR}/UI/ModelImporterInspector.*
    )
FILE(GLOB EditorFiles
    ${FishEditor_qt_SRC_DIR}/Editor.hpp
    ${FishEditor_qt_SRC_DIR}/Editor.cpp
    ${FishEditor_qt_SRC_DIR}/SerializedObject.*
    ${FishEditor_qt_SRC_DIR}/SerializedProperty.*
    ${FishEditor_qt_SRC_DIR}/SerializedPropertyType.hpp
    ${FishEditor_qt_SRC_DIR}/EditorGUILayout.*)


# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets)

#FILE(GLOB ReflectHeaders ${FishEditor_qt_SRC_DIR}/generate/*.hpp)
FILE(GLOB ReflectFilesSources ${FishEditor_qt_SRC_DIR}/generate/*.hpp ${FishEditor_qt_SRC_DIR}/generate/*.cpp)
SOURCE_GROUP(Reflect FILES ${ReflectFilesSources})

# FILE(GLOB InternalSources ${FishEditor_qt_SRC_DIR}/Internal/*.hpp ${FishEditor_qt_SRC_DIR}/Internal/*.cpp )
# SOURCE_GROUP(Internal FILES ${InternalSources})

FILE(GLOB Asset_SRCS ${FishEditor_qt_SRC_DIR}/FBXImporter/*.hpp ${FishEditor_qt_SRC_DIR}/FBXImporter/*.cpp)
foreach (x AssetArchive AssetDataBase SceneArchive AssetImporter TextureImporter ModelImporter FBXImporter ShaderImporter DDSImporter AudioImporter)
    foreach (ext hpp cpp)
        set(f ${FishEditor_qt_SRC_DIR}/${x}.${ext})
        SET(Asset_SRCS ${Asset_SRCS} ${f})
        IF(NOT EXISTS ${f})
            MESSAGE(FATAL_ERROR "source files does not exist: ${f}")
        ENDIF ()
    endforeach ()
endforeach()

SET( ASSETS_DIR ${FishEditor_qt_SRC_DIR}/../../../assets)
SET( SHADER_DIR ${FishEditor_qt_SRC_DIR}/../../Shaders)
IF( NOT WIN32 )
    FILE( GLOB Models ${ASSETS_DIR}/Models/*.txt )
    FILE( GLOB Gizmos ${ASSETS_DIR}/Textures/Gizmos/*.png )
    FILE( GLOB Shader_headers ${SHADER_DIR}/include/*.inc )
    FILE( GLOB Editor_shaders ${SHADER_DIR}/Editor/*.* )
    FILE( GLOB Shaders ${SHADER_DIR}/*.* )
    SET( SCRIPTS ${FISHENGINE_SCRIPT_DIR}/Editor/BuildProject.py )
    SET( RESOURCE_FILES ${Models} ${Gizmos} ${Shaders} ${Shader_headers} ${Editor_shaders} ${SCRIPTS})
ENDIF()

SOURCE_GROUP( Asset FILES ${Asset_SRCS} )
SOURCE_GROUP( UI    FILES ${UI_SRCS} ${UI_Heasers} )
SOURCE_GROUP( MOC   FILES ${MOC_SRCS} )
SOURCE_GROUP( Resources FILES ${RESOURCE_FILES} )
SOURCE_GROUP( Inspector FILES ${InspectorFiles} )
SOURCE_GROUP( Editor FILES ${EditorFiles})

add_executable(FishEditor_qt MACOSX_BUNDLE ${SRCS} ${Heasers} ${UI_SRCS} ${UI_Heasers} ${FORMS} ${InspectorFiles} ${ReflectFilesSources} ${EditorFiles} ${FishEditor_qt_SRC_DIR}/resources.qrc ${RESOURCE_FILES} ${Asset_SRCS} ${InternalSources})

SET( INFO_PLIST "${FishEditor_qt_SRC_DIR}/../../CMake/MacOSXBundleInfo.plist.in")
IF (NOT EXISTS ${INFO_PLIST})
    MESSAGE(FATAL_ERROR "${INFO_PLIST} not found!")
ENDIF()
set_target_properties( FishEditor_qt PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${INFO_PLIST}" )

IF( NOT WIN32 )
    SET_SOURCE_FILES_PROPERTIES(${Models} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Models)
    SET_SOURCE_FILES_PROPERTIES(${Gizmos} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/Textures/Gizmos)
    SET_SOURCE_FILES_PROPERTIES(${Shaders} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders)
    SET_SOURCE_FILES_PROPERTIES(${Shader_headers} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders/include)
    SET_SOURCE_FILES_PROPERTIES(${Editor_shaders} PROPERTIES MACOSX_PACKAGE_LOCATION Shaders/Editor)
    SET_SOURCE_FILES_PROPERTIES(${SCRIPTS} PROPERTIES MACOSX_PACKAGE_LOCATION .)
ENDIF()

# IF( WIN32 )
#     target_link_libraries(FishEditor_qt filesystem)
#     target_link_libraries(FishEditor_qt system)
# ENDIF()
target_link_libraries(FishEditor_qt FishEngine)
#target_link_libraries(FishEditor_qt assimp)
#target_link_libraries(FishEditor_qt yaml-cpp)
#target_link_libraries(FishEditor_qt ${PhysXSDK_LIBRARIES})
#target_link_libraries(FishEditor_qt ${Boost_LIBRARIES})
target_link_libraries(FishEditor_qt ${FBXSDK_LIB})
target_link_libraries(FishEditor_qt freeimage)
target_link_libraries(FishEditor_qt ${PYTHON3_LIB})

if (MSVC)
    # target_link_libraries(FishEditor_qt opengl32.lib)
    # target_link_libraries(FishEditor_qt glew_s)
else()
    set_target_properties(FishEditor_qt PROPERTIES LINK_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
endif()

target_link_libraries(FishEditor_qt Qt5::Widgets)


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

    qt5_copy_dll(FishEditor_qt Core)
    qt5_copy_dll(FishEditor_qt Gui)
    qt5_copy_dll(FishEditor_qt Widgets)

    set(PhysX_BIN_DIR ${PhysX_ROOT_DIR}/Bin/vc14win64)
    message(${PhysX_BIN_DIR}/nvToolsExt64_1.dll)
    message(${PhysX_BIN_DIR}/PhysX3CommonDEBUG_x64.dll)
    message(${PhysX_BIN_DIR}/PhysX3DEBUG_x64.dll)

    add_custom_command(TARGET FishEditor_qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PhysX_BIN_DIR}/nvToolsExt64_1.dll $<TARGET_FILE_DIR:FishEditor_qt>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${PhysX_BIN_DIR}/PhysX3CommonDEBUG_x64.dll> $<$<NOT:$<CONFIG:Debug>>:${PhysX_BIN_DIR}/PhysX3CommonCHECKED_x64.dll> $<TARGET_FILE_DIR:FishEditor_qt>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${PhysX_BIN_DIR}/PhysX3DEBUG_x64.dll> $<$<NOT:$<CONFIG:Debug>>:${PhysX_BIN_DIR}/PhysX3CHECKED_x64.dll> $<TARGET_FILE_DIR:FishEditor_qt>
    )

    add_custom_command(TARGET FishEditor_qt POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FreeImage_Root}/bin/FreeImage.dll $<TARGET_FILE_DIR:FishEditor_qt>)

    add_custom_command(TARGET FishEditor_qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} $<TARGET_FILE_DIR:FishEditor_qt>/Resources
    )

    add_custom_command(TARGET FishEditor_qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} $<TARGET_FILE_DIR:FishEditor_qt>/Shaders
    )

    set(EDITOR_SCRIPT ${FISHENGINE_SCRIPT_DIR}/Editor)
    add_custom_command(TARGET FishEditor_qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EDITOR_SCRIPT}/BuildProject.py $<TARGET_FILE_DIR:FishEditor_qt>/BuildProject.py
    )
endif()


