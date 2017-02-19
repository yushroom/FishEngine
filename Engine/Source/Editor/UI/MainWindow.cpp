#include <GLEnvironment.hpp>
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QStandardItemModel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <FishEngine.hpp>
#include <Debug.hpp>
#include <Resources.hpp>
#include <RenderSystem.hpp>
#include <Input.hpp>
#include <Application.hpp>
#include <Camera.hpp>
#include <Scene.hpp>

#include "EditorResources.hpp"
#include "Inspector.hpp"
#include "MainEditor.hpp"
#include "SceneViewEditor.hpp"
#include "FileInfo.hpp"
#include "Selection.hpp"

#include <fstream>
#include <Serialization.hpp>
#include <GameObject.hpp>
#include <Serialization/archives/yaml.hpp>
#include <Timer.hpp>

using namespace FishEngine;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //FishEngine::Debug::Log("MainWindow::ctor");
    Init();
	this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	
    ui->setupUi(this);
	//this->setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);

    ui->sceneView->setFocus();

    FishEditor::Inspector::s_inspectorWidget = ui->inspectorWidget;

	connect(ui->actionNewScene, &QAction::triggered, this, &MainWindow::NewScene);
    connect(ui->actionSaveSceneAs, &QAction::triggered, this, &MainWindow::SaveSceneAs);

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

    connect(ui->actionFrameSelected, &QAction::triggered, [](){
        //FishEngine::Camera::main()->FrameSelected(FishEditor::Selection::activeGameObject());
        FishEditor::MainEditor::m_mainSceneViewEditor->FrameSelected(FishEditor::Selection::activeGameObject());
    });

//    FishEditor::MainEditor::OnInitialized += [this](){
//        ui->projectView->SetRootPath(FishEngine::Applicaiton::dataPath());
//    };
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Init()
{
    FishEngine::Debug::Init();

    Applicaiton::s_isEditor = true;

	//FishEngine::Timer t("Load assets");
	FishEditor::FileInfo::SetAssetRootPath(Applicaiton::s_dataPath);
	//t.StopAndPrint();

	//// http://stackoverflow.com/questions/37987426/qt-non-blocking-overlay-dialog
	//auto dialog = new SelectObjectDialog(this);
	//auto flags = dialog->windowFlags();
	//dialog->setWindowFlags(flags | Qt::Tool);
	//dialog->show();
}

bool MainWindow::CloseCurrentScene()
{
	int btn = QMessageBox::warning(this,
		"Scene Have Been Modified",
		"Do you want to save the changes you made in the scenes: \n Assets/<balbal.scene>\n\nYour changes will be lost if you don't save them.", "Save", "Don't Save", "Cancel", 0, 2);
	if (btn == 0)	// save
	{
		return true;
	}
	else if (btn == 1) // don't save
	{
		return true;
	}

	return false;
}

void MainWindow::NewScene()
{
	CloseCurrentScene();
}

void MainWindow::SaveSceneAs()
{
    //options |= QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString path = QString::fromStdString(FishEngine::Applicaiton::dataPath().string());
    path = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene (*.scene)");
    if (path.isEmpty())
        return;
    std::ofstream fout(path.toStdString());
    FishEngine::YAMLOutputArchive archive(fout);
    for (auto const & go : Scene::GameObjects())
    {
        archive << go;
    }
}
