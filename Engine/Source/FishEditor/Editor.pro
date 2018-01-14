#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T22:21:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = Editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

MOC_DIR = ./moc
UI_DIR = ./ui
OBJECTS_DIR = ./obj

win32 {
    INCLUDEPATH += C:\Boost\include\boost-1_61  #/usr/local/include	#boost
    LIBS += -LC:\Boost\lib
    LIBS += C:\Boost\lib\libboost_system-vc140-mt-1_61.lib C:\Boost\lib\libboost_filesystem-vc140-mt-1_61.lib
    LIBS += -lopengl32
}

INCLUDEPATH += ../Runtime \
    ../../ThirdParty/ \
    ../../ThirdParty/PhysXSDK/Include \
    ../../ThirdParty/stb \
    ../../ThirdParty/assimp-3.3.1/include \
    ../../ThirdParty/yaml-cpp/include \
    ../../ThirdParty/glew-2.0.0/include

LIBS += -L../../Binary/lib/RelWithDebInfo -L../../ThirdParty/PhysXSDK/Lib/vc14win64 -L../../build/lib/RelWithDebInfo
LIBS += -lFishEngine -llibyaml-cppmd -lassimp-vc140-mt -llibglew32 -lzlibstatic
LIBS += -lPhysX3CHECKED_x64 -lPhysX3CommonCHECKED_x64 -lPhysX3CookingCHECKED_x64
LIBS += -lPhysX3ExtensionsCHECKED -lPhysX3VehicleCHECKED -lPhysXProfileSDKCHECKED -lPhysXVisualDebuggerSDKCHECKED -lPxTaskCHECKED

SOURCES += \
    generate/EditorClassSerialization.cpp \
    UI/AssetProgressDialog.cpp \
    UI/ClickableLabel.cpp \
    UI/FloatLineEdit.cpp \
    UI/GLWidget.cpp \
    UI/HierarchyTreeView.cpp \
    UI/InspectorWidget.cpp \
    UI/MainWindow.cpp \
    UI/ObjectListModel.cpp \
    UI/ProjectView.cpp \
    UI/ProjectViewFileModel.cpp \
    UI/SelectObjectDialog.cpp \
    UI/UIAssetHeader.cpp \
    UI/UIBool.cpp \
    UI/UIButton.cpp \
    UI/UIColor.cpp \
    UI/UIComboBox.cpp \
    UI/UIComponentHeader.cpp \
    UI/UIFloat.cpp \
    UI/UIFloat3.cpp \
    UI/UIFloat4.cpp \
    UI/UIGameObjectHeader.cpp \
    UI/UIInt.cpp \
    UI/UIMaterialHeader.cpp \
    UI/UIObjecField.cpp \
    UI/UISlider.cpp \
    UI/UIString.cpp \
    UI/UITexture.cpp \
    App.cpp \
    AssetDataBase.cpp \
    AssetImporter.cpp \
    EditorGUI.cpp \
    FileInfo.cpp \
    Inspector.cpp \
    main.cpp \
    MainEditor.cpp \
    ModelImporter.cpp \
    ProjectSettings.cpp \
    SceneViewEditor.cpp \
    Selection.cpp \
    TextureImporter.cpp \
    UI/OpenProjectDialog.cpp \
    UI/ProjectListView.cpp \
    UI/LogView.cpp

HEADERS += \
    generate/Enum_ImportAssetOptions.hpp \
    generate/Enum_ModelImporterAnimationCompression.hpp \
    generate/Enum_ModelImporterAnimationType.hpp \
    generate/Enum_ModelImporterMaterialName.hpp \
    generate/Enum_ModelImporterMaterialSearch.hpp \
    generate/Enum_ModelImporterMeshCompression.hpp \
    generate/Enum_ModelImporterNormals.hpp \
    generate/Enum_ModelImporterTangents.hpp \
    generate/Enum_ShadingMode.hpp \
    generate/Enum_TextureImporterAlphaSource.hpp \
    generate/Enum_TextureImporterCompression.hpp \
    generate/Enum_TextureImporterGenerateCubemap.hpp \
    generate/Enum_TextureImporterMipFilter.hpp \
    generate/Enum_TextureImporterNPOTScale.hpp \
    generate/Enum_TextureImporterShape.hpp \
    generate/Enum_TextureImporterType.hpp \
    generate/Enum_TransformSpace.hpp \
    generate/Enum_TransformToolType.hpp \
    generate/Enum_UIHeaderState.hpp \
    UI/AssetProgressDialog.hpp \
    UI/ClickableLabel.hpp \
    UI/FloatLineEdit.hpp \
    UI/GLWidget.hpp \
    UI/HierarchyTreeView.hpp \
    UI/InspectorWidget.hpp \
    UI/MainWindow.hpp \
    UI/ObjectListModel.hpp \
    UI/ProjectView.hpp \
    UI/ProjectViewFileModel.hpp \
    UI/SelectObjectDialog.hpp \
    UI/UIAssetHeader.hpp \
    UI/UIBool.hpp \
    UI/UIButton.hpp \
    UI/UIColor.hpp \
    UI/UIComboBox.hpp \
    UI/UIComponentHeader.hpp \
    UI/UIDebug.hpp \
    UI/UIFloat.hpp \
    UI/UIFloat3.hpp \
    UI/UIFloat4.hpp \
    UI/UIGameObjectHeader.hpp \
    UI/UIHeaderState.hpp \
    UI/UIInt.hpp \
    UI/UIMaterialHeader.hpp \
    UI/UIObjecField.hpp \
    UI/UISlider.hpp \
    UI/UIString.hpp \
    UI/UITexture.cpp.n31036 \
    UI/UITexture.hpp \
    App.hpp \
    AssetDataBase.hpp \
    AssetImporter.hpp \
    EditorClassID.hpp \
    EditorGUI.hpp \
    EditorUtility.hpp \
    FileInfo.hpp \
    FishEditor.hpp \
    Helper.hpp \
    Inspector.hpp \
    MainEditor.hpp \
    ModelImporter.hpp \
    ProjectSettings.hpp \
    SceneViewEditor.hpp \
    Selection.hpp \
    TextureImporter.hpp \
    TextureImporterProperties.hpp \
    UI/OpenProjectDialog.hpp \
    UI/ProjectListView.hpp \
    UI/LogView.hpp

FORMS   += \
    UI/AssetProgressDialog.ui \
    UI/MainWindow.ui \
    UI/ProjectView.ui \
    UI/SelectObjectDialog.ui \
    UI/UIAssetHeader.ui \
    UI/UIBool.ui \
    UI/UIColor.ui \
    UI/UIComboBox.ui \
    UI/UIComponentHeader.ui \
    UI/UIFloat.ui \
    UI/UIFloat3.ui \
    UI/UIFloat4.ui \
    UI/UIGameObjectHeader.ui \
    UI/UIInt.ui \
    UI/UIMaterialHeader.ui \
    UI/UIObjecField.ui \
    UI/UISlider.ui \
    UI/UIString.ui \
    UI/UITexture.ui \
    UI/OpenProjectDialog.ui \
    UI/LogView.ui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12 # no effect, set QMAKE_MACOSX_DEPLOYMENT_TARGET in <QtDir>/clang_64/mkspecs/macx-clang/qmake.conf instead

DISTFILES +=

RESOURCES += \
    resources.qrc
