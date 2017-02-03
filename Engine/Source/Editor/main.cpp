#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
//#include <Resources.hpp>

int main(int argc, char *argv[])
{
    //FishEngine::Resources::SetAssetsDirectory("/Users/yushroom/program/graphics/FishEngine/Example/Sponza");

    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.show();

    return a.exec();
}
