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


INCLUDEPATH += ../Runtime \
    ../../ThirdParty/PhysXSDK/Include \
    ../../ThirdParty/stb \
    /usr/local/include	#boost

LIBS += ../../Binary/lib/RelWithDebInfo/libFishEngine.a \
    ../../Binary/lib/RelWithDebInfo/libyaml-cpp.a \
    ../../Binary/lib/RelWithDebInfo/libassimp.a \
    ../../ThirdParty/PhysXSDK/Lib/osx64/*.a \
    /usr/local/lib/libboost_filesystem-mt.a \
    /usr/local/lib/libboost_system-mt.a \
    -lz

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    App.cpp \
    Selection.cpp \
    SceneViewEditor.cpp \
    EditorGUI.cpp \
    UI/UIComboBox.cpp \
    UI/UIBool.cpp \
    UI/UIFloat3.cpp \
    UI/UIComponentHeader.cpp \
    UI/UIInt.cpp \
    UI/UIString.cpp \
    UI/UIFloat.cpp \
    UI/UISlider.cpp \
    UI/UIObjecField.cpp \
    UI/UIColor.cpp \
    HierarchyTreeView.cpp \
    MainEditor.cpp \
    AddComponentDialog.cpp \
    UI/UIButton.cpp \
    UI/FloatLineEdit.cpp \
    ProjectView.cpp \
    UI/OpenProjectDialog.cpp \
    ProjectFileIconProvider.cpp \
    ProjectViewFileModel.cpp \
    FileInfo.cpp \
    UI/SelectObjectDialog.cpp \
    ObjectListModel.cpp \
    UI/UIMaterialHeader.cpp \
    UI/InspectorWidget.cpp \
    UI/UIGameObjectHeader.cpp \
    Inspector.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    App.hpp \
    Selection.hpp \
    FishEditor.hpp \
    SceneViewEditor.hpp \
    EditorGUI.hpp \
    UI/UIComboBox.hpp \
    UI/UIBool.hpp \
    UI/UIFloat3.hpp \
    UI/UIComponentHeader.hpp \
    UI/UIInt.hpp \
    UI/UIString.hpp \
    UI/UIFloat.hpp \
    UI/UISlider.hpp \
    UI/UIObjecField.hpp \
    UI/UIColor.hpp \
    HierarchyTreeView.hpp \
    MainEditor.hpp \
    AddComponentDialog.hpp \
    UI/UIButton.hpp \
    UIDebug.hpp \
    UIHeaderState.hpp \
    UI/FloatLineEdit.hpp \
    ProjectView.hpp \
    UI/OpenProjectDialog.hpp \
    ProjectFileIconProvider.hpp \
    ProjectViewFileModel.hpp \
    FileInfo.hpp \
    UI/SelectObjectDialog.hpp \
    ObjectListModel.hpp \
    UI/UIMaterialHeader.hpp \
    Inspector.hpp \
    UI/InspectorWidget.hpp \
    UI/UIGameObjectHeader.hpp

FORMS    += mainwindow.ui \
    UI/UIComboBox.ui \
    UI/UIBool.ui \
    UI/UIFloat3.ui \
    UI/UIComponentHeader.ui \
    UI/UIInt.ui \
    UI/UIString.ui \
    UI/UIFloat.ui \
    UI/UISlider.ui \
    UI/UIObjecField.ui \
    UI/UIColor.ui \
    AddComponentDialog.ui \
    ProjectView.ui \
    UI/OpenProjectDialog.ui \
    UI/SelectObjectDialog.ui \
    UI/UIMaterialHeader.ui \
    UI/UIGameObjectHeader.ui

#QMAKE_CXXFLAGS += -std=c++14
#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12 # no effect, set QMAKE_MACOSX_DEPLOYMENT_TARGET in <QtDir>/clang_64/mkspecs/macx-clang/qmake.conf instead

DISTFILES +=

RESOURCES += \
    resources.qrc
