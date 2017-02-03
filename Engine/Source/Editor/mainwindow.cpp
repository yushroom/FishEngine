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
#include <Application.hpp>

#include "GameObjectInspector.hpp"
#include "MainEditor.hpp"
#include "SceneViewEditor.hpp"

using namespace FishEngine;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //FishEngine::Debug::Log("MainWindow::ctor");
    Init();
    ui->setupUi(this);

    m_gameObjectInspector = new GameObjectInspector;
    ui->InspectorContents->layout()->addWidget(m_gameObjectInspector);

    connect(ui->actionPlay, &QAction::triggered, [this](){
        if (Applicaiton::isPlaying())
        {
            //this->ui->actionPlay->setText("Play");
            this->ui->actionPlay->setIcon(QIcon(":/Resources/Play_black.png"));
            //this->ui->actionPlay->set
            FishEditor::MainEditor::Stop();
        }
        else
        {
            //this->ui->actionPlay->setText("Stop");
            this->ui->actionPlay->setIcon(QIcon(":/Resources/Stop_100.png"));
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

//    FishEditor::MainEditor::OnInitialized += [this](){
//        ui->projectView->SetRootPath(FishEngine::Applicaiton::dataPath());
//    };
}

MainWindow::~MainWindow()
{
    delete ui;
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
