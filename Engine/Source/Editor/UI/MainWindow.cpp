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
//#include <Serialization/archives/yaml.hpp>
#include <Timer.hpp>

#include "SceneArchive.hpp"

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

	connect(ui->actionOpenScene, &QAction::triggered, this, &MainWindow::OpenScene);
	connect(ui->actionSaveSceneAs, &QAction::triggered, this, &MainWindow::SaveSceneAs);

	connect(ui->actionPlay, &QAction::triggered, [this](){
		if (Applicaiton::isPlaying())
		{
			this->ui->actionPlay->setIcon(QIcon(":/Resources/Play_black.png"));
			this->ui->actionPlay->setChecked(true);
			FishEditor::MainEditor::Stop();
		}
		else
		{
			this->ui->actionPlay->setIcon(QIcon(":/Resources/Stop_100.png"));
			this->ui->actionPlay->setChecked(false);
			FishEditor::MainEditor::Play();
		}
	});

	this->ui->actionLocalOrGlobal->setText("Global");
	//FishEditor::MainEditor::m_mainSceneViewEditor->setTransformSpace(FishEditor::TransformSpace::Global);

	connect(ui->actionLocalOrGlobal, &QAction::triggered, [this]() {
		auto space = FishEditor::MainEditor::m_mainSceneViewEditor->transformSpace();

		if (space == FishEditor::TransformSpace::Local)
		{
			this->ui->actionLocalOrGlobal->setText("Global");
			FishEditor::MainEditor::m_mainSceneViewEditor->setTransformSpace(FishEditor::TransformSpace::Global);
		}
		if (space == FishEditor::TransformSpace::Global)
		{
			this->ui->actionLocalOrGlobal->setText("Local");
			FishEditor::MainEditor::m_mainSceneViewEditor->setTransformSpace(FishEditor::TransformSpace::Local);
		}
	});

	connect(ui->actionHand, &QAction::triggered, [this](){
		FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::None);
		//this->ui->actionHand->setChecked(false);
		this->ui->actionTranslate->setChecked(false);
		this->ui->actionRotation->setChecked(false);
		this->ui->actionScale->setChecked(false);
		this->ui->actionHand->setChecked(true);
	});

	connect(ui->actionTranslate, &QAction::triggered, [this](){
		FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Translate);
		this->ui->actionHand->setChecked(false);
		//this->ui->actionTranslate->setChecked(false);
		this->ui->actionRotation->setChecked(false);
		this->ui->actionScale->setChecked(false);
		this->ui->actionTranslate->setChecked(true);
	});

	connect(ui->actionRotation, &QAction::triggered, [this](){
		FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Rotate);
		this->ui->actionHand->setChecked(false);
		this->ui->actionTranslate->setChecked(false);
		//this->ui->actionRotation->setChecked(false);
		this->ui->actionScale->setChecked(false);
		this->ui->actionRotation->setChecked(true);
	});

	connect(ui->actionScale, &QAction::triggered, [this](){
		FishEditor::MainEditor::m_mainSceneViewEditor->setTransformToolType(FishEditor::TransformToolType::Scale);
		this->ui->actionHand->setChecked(false);
		this->ui->actionTranslate->setChecked(false);
		this->ui->actionRotation->setChecked(false);
		//this->ui->actionScale->setChecked(false);
		this->ui->actionScale->setChecked(true);
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

void MainWindow::OpenScene()
{
	QString path = QString::fromStdString(FishEngine::Applicaiton::dataPath().string());
	path = QFileDialog::getOpenFileName(this, "Load Scene", path, "Scene (*.scene)");
	if (path.isEmpty())
	{
		return;
	}
	std::ifstream fin(path.toStdString());
	FishEditor::SceneInputArchive archive(fin);
}

void MainWindow::SaveSceneAs()
{
	//options |= QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString path = QString::fromStdString(FishEngine::Applicaiton::dataPath().string());
	path = QFileDialog::getSaveFileName(this, "Save Scene", path, "Scene (*.scene)");
	if (path.isEmpty())
	{
		return;
	}
	{
		std::ofstream fout(path.toStdString());
		FishEditor::SceneOutputArchive archive(fout);
		for (auto const & go : Scene::GameObjects())
		{
			archive << go;
		}
	}

	{
		auto metaPath = path.toStdString() + ".meta";
		std::ofstream fout(metaPath);
		uint32_t time_created = static_cast<uint32_t>(time(NULL));
		FishEditor::AssetOutputArchive archive(fout);
		archive.BeginMap();
		archive << make_nvp("fileFormatVersion", 2);
		archive << make_nvp("guid", "xxxxxxxx");
		archive << make_nvp("timeCreated", time_created);
		archive.EndMap();
	}
}
