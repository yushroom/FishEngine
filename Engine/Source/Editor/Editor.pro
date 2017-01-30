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


INCLUDEPATH += /Users/yushroom/program/graphics/FishEngine/Engine/Source/Runtime \
    #/Users/yushroom/program/graphics/FishEngine/Engine/ThirdParty/imgui \
    /usr/local/include

LIBS += /Users/yushroom/program/graphics/FishEngine/Engine/Binary/lib/RelWithDebInfo/libFishEngine.a \
    /Users/yushroom/program/graphics/FishEngine/Engine/Binary/lib/RelWithDebInfo/libyaml-cpp.a \
    /Users/yushroom/program/graphics/FishEngine/Engine/Binary/lib/RelWithDebInfo/libassimp.a \
    #/Users/yushroom/program/graphics/FishEngine/Engine/Binary/lib/RelWithDebInfo/libimgui.a \
    /Users/yushroom/program/graphics/FishEngine/Engine/ThirdParty/PhysXSDK/Lib/osx64/*.a \
    /usr/local/lib/libboost_filesystem-mt.a \
    /usr/local/lib/libboost_system-mt.a \
    -lz

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    App.cpp \
    Selection.cpp \
    SceneViewEditor.cpp \
    GameObjectInspector.cpp \
    HierarchyView.cpp \
    EditorGUI.cpp \
    UI/UIComboBox.cpp \
    UI/UIBool.cpp \
    UI/UIFloat3.cpp \
    UI/UIHeader.cpp \
    UI/UIInt.cpp \
    UI/UIString.cpp \
    UI/UIFloat.cpp \
    UI/UISlider.cpp \
    UI/UIObjecField.cpp \
    UI/UIColor.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    App.hpp \
    Selection.hpp \
    FishEditor.hpp \
    SceneViewEditor.hpp \
    GameObjectInspector.hpp \
    HierarchyView.hpp \
    EditorGUI.hpp \
    UI/UIComboBox.hpp \
    UI/UIBool.hpp \
    UI/UIFloat3.hpp \
    UI/UIHeader.hpp \
    UI/UIInt.hpp \
    UI/UIString.hpp \
    UI/UIFloat.hpp \
    UI/UISlider.hpp \
    UI/UIObjecField.hpp \
    UI/UIColor.hpp

FORMS    += mainwindow.ui \
    GameObjectInspector.ui \
    HierarchyView.ui \
    UI/UIComboBox.ui \
    UI/UIBool.ui \
    UI/UIFloat3.ui \
    UI/UIHeader.ui \
    UI/UIInt.ui \
    UI/UIString.ui \
    UI/UIFloat.ui \
    UI/UISlider.ui \
    UI/UIObjecField.ui \
    UI/UIColor.ui

#QMAKE_CXXFLAGS += -std=c++14
#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12 # no effect, set QMAKE_MACOSX_DEPLOYMENT_TARGET in <QtDir>/clang_64/mkspecs/macx-clang/qmake.conf instead

DISTFILES +=
