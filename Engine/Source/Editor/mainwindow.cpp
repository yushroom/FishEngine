#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileIconProvider>
#include <QMouseEvent>
#include <QStandardItemModel>
//#include <QPalette>

#include <FishEngine.hpp>
#include <Debug.hpp>
#include <Resources.hpp>
#include <RenderSystem.hpp>
#include <Input.hpp>

#include "GameObjectInspector.hpp"
#include "MainEditor.hpp"
#include "SceneViewEditor.hpp"

using namespace FishEngine;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    FishEngine::Debug::Log("MainWindow::ctor");
    Init();
    ui->setupUi(this);

    auto tree = ui->treeView;
    const QString dir = "/Users/yushroom/program/graphics/FishEngine/Example";

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath(dir);

    tree->setModel(dirModel);
    const QModelIndex rootIndex = dirModel->index(QDir::cleanPath(dir));
    if (rootIndex.isValid())
        tree->setRootIndex(rootIndex);

    for (int i = 1; i < tree->header()->count(); ++i)
    {
        tree->hideColumn(i);
    }
    tree->header()->hide();

    auto listView = ui->listView;

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(dir);

    listView->setModel(fileModel);
    listView->setViewMode(QListView::IconMode);
    listView->setIconSize(QSize(64, 64));
    listView->setRootIndex(fileModel->setRootPath(dir));

    m_gameObjectInspector = new GameObjectInspector;
    ui->InspectorContents->layout()->addWidget(m_gameObjectInspector);

    connect(ui->actionPlay, &QAction::triggered, [this](){
        if (FishEditor::MainEditor::InPlayMode())
        {
            this->ui->actionPlay->setText("Play");
            FishEditor::MainEditor::Stop();
        }
        else
        {
            this->ui->actionPlay->setText("Stop");
            FishEditor::MainEditor::Play();
        }
    });

    connect(ui->actionHand, &QAction::triggered, [](){
        FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::None);
    });

    connect(ui->actionTranslate, &QAction::triggered, [](){
        FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Translate);
    });

    connect(ui->actionRotation, &QAction::triggered, [](){
        FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Rotate);
    });

    connect(ui->actionScale, &QAction::triggered, [](){
        FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Scale);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    auto path = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(path));
}

//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    Q_UNUSED(watched);
//    auto type = event->type();
//    if (type == QEvent::MouseMove)
//    {
//        //auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
//        //statusBar()->showMessage(QString("Mouse move (%1 %2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
//    }
//    else if (type == QEvent::MouseButtonPress)
//    {
//        statusBar()->showMessage("MousePress");
//    }
//    else if (type == QEvent::MouseButtonRelease)
//    {
//        statusBar()->showMessage("MouseRelease");
//    }

//    return false;
//}

void MainWindow::Init()
{
    FishEngine::Debug::Init();
    FishEngine::Resources::Init("/Users/yushroom/program/graphics/FishEngine/Engine");
    //FishEngine::Input::Init();

//    RenderSystem::Init();
}
