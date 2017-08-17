#include <FishEngine/GLEnvironment.hpp>
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QStandardItemModel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <FishEngine/FishEngine.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Resources.hpp>
#include <FishEngine/RenderSystem.hpp>
#include <FishEngine/Input.hpp>
#include <FishEngine/Application.hpp>
#include <FishEngine/Camera.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Timer.hpp>
#include <FishEngine/Path.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/ShaderCompiler.hpp>

#include "EditorResources.hpp"
#include "Inspector.hpp"
#include "MainEditor.hpp"
#include "SceneViewEditor.hpp"
#include "FileInfo.hpp"
#include "Selection.hpp"
#include "EditorApplication.hpp"

#include <fstream>

#include "SceneArchive.hpp"
#include "AssetArchive.hpp"
#include "LogView.hpp"

#include <folly/dynamic.h>
#include <folly/json.h>

using namespace FishEngine;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	//FishEngine::Debug::Log("MainWindow::ctor");
	Init();
	this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	
	ui->setupUi(this);
#if FISHENGINE_PLATFORM_WINDOWS
	ui->actionDelete->setShortcut(QApplication::translate("MainWindow", "Delete", Q_NULLPTR));
#endif

	ui->sceneView->setFocus();

	FishEditor::Inspector::s_inspectorWidget = ui->inspectorWidget;

	connect(ui->actionNewScene, &QAction::triggered, this, &MainWindow::NewScene);

	connect(ui->actionOpenScene, &QAction::triggered, this, &MainWindow::OpenScene);
	connect(ui->actionSaveSceneAs, &QAction::triggered, this, &MainWindow::SaveSceneAs);

	connect(ui->actionPlay, &QAction::triggered, [this](){
		if (Application::isPlaying())
		{
			this->ui->actionPlay->setIcon(QIcon(":/Resources/toolbar_playbutton.png"));
			this->ui->actionPlay->setChecked(false);
			FishEditor::MainEditor::Stop();
		}
		else
		{
			this->ui->actionPlay->setIcon(QIcon(":/Resources/toolbar_playbutton_on.png"));
			this->ui->actionPlay->setChecked(true);
			FishEditor::MainEditor::Play();
		}
	});

	//auto space = FishEditor::MainEditor::m_mainSceneViewEditor->transformSpace();
	//if (space == FishEditor::TransformSpace::Global)
	//	this->ui->actionLocalOrGlobal->setText("Global");
	//else
	//	this->ui->actionLocalOrGlobal->setText("Local");
	////FishEditor::MainEditor::m_mainSceneViewEditor->setTransformSpace(FishEditor::TransformSpace::Global);

	//auto pivot = FishEditor::MainEditor::m_mainSceneViewEditor->transformPivot();
	//if (pivot == FishEditor::TransformPivot::Center)
	//	this->ui->actionCenterOrPivot->setText("Center");
	//else
	//	this->ui->actionCenterOrPivot->setText("Pivot");
	////FishEditor::MainEditor::m_mainSceneViewEditor->setTransformPivot(FishEditor::TransformPivot::Center);

	this->ui->actionLocalOrGlobal->setText("Global");
	this->ui->actionCenterOrPivot->setText("Center");

	connect(ui->actionCenterOrPivot, &QAction::triggered, [this]() {
		auto pivot = FishEditor::MainEditor::m_mainSceneViewEditor->transformPivot();

		if (pivot == FishEditor::TransformPivot::Pivot)
		{
			this->ui->actionCenterOrPivot->setText("Center");
			FishEditor::MainEditor::m_mainSceneViewEditor->setTransformPivot(FishEditor::TransformPivot::Center);
		}
		if (pivot == FishEditor::TransformPivot::Center)
		{
			this->ui->actionCenterOrPivot->setText("Pivot");
			FishEditor::MainEditor::m_mainSceneViewEditor->setTransformPivot(FishEditor::TransformPivot::Pivot);
		}
	});

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
		FishEditor::MainEditor::m_mainSceneViewEditor->FrameSelected();
	});
	
	connect(ui->sceneViewDrawModeButton, &QToolButton::clicked, this, &MainWindow::ShowSceneViewDrawModeMenu);
	
	connect(ui->actionDelete,
			&QAction::triggered,
			ui->hierarchyTreeView->m_deleteAction,
			&QAction::trigger);
	
	connect(ui->actionDuplicate,
			&QAction::triggered,
			ui->hierarchyTreeView->m_duplicateAction,
			&QAction::trigger);

//    FishEditor::MainEditor::OnInitialized += [this](){
//        ui->projectView->SetRootPath(FishEngine::Application::dataPath());
//    };
	
//	auto view = new LogView();
//	view->show();
	
	//OpenScene();
	// 先定义一个json字符串
	std::string jsonDocument = R"({"key":12,"key2":[false, null, true, "yay"]})";
	// 执行json反序列化，反序列化结果为dynamic
	folly::dynamic parsed = folly::parseJson(jsonDocument);
	assert(parsed["key"] == 12);
	assert(parsed["key2"][0] == false);
	assert(parsed["key2"][1] == nullptr);
	
	// 构建一个OBJECT
	folly::dynamic sonOfAJ = folly::dynamic::object
	("key", 12)
	("key2", folly::dynamic::array(false, nullptr, true, "yay"));
	
	// json序列化
	auto str = folly::toJson(sonOfAJ);
	std::cout << jsonDocument << std::endl;
	std::cout << str << std::endl;
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::Init()
{
	FishEngine::Debug::Init();

	Application::s_isEditor = true;

	QDir cwd = QCoreApplication::applicationDirPath();
#if FISHENGINE_PLATFORM_APPLE
	cwd.cdUp();
#endif

	auto & editorPath = FishEditor::EditorApplication::m_applicationPath;
	editorPath = FishEngine::Path(cwd.absolutePath().toStdString());

	//s_rootSystemDirectory = cwd.absolutePath().toStdString();
	auto shaderRoot = editorPath / "shaders";
	auto shaderIncludeDir = shaderRoot / "include";
	//auto p = FishEngine::Path(cwd.absolutePath().toStdString()) / "shaders" / "include";
	ShaderCompiler::setShaderIncludeDir(shaderIncludeDir.string());
	//ShaderCompiler::s_shaderIncludeDir = shaderRootDirectory() / "include";
	
	Shader::Init(shaderRoot.string());

	//FishEngine::Timer t("Load assets");
	FishEditor::FileInfo::SetAssetRootPath(Application::s_dataPath);
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
	QString path = QString::fromStdString(FishEngine::Application::dataPath().string());
	path = QFileDialog::getOpenFileName(this, "Load Scene", path, "Scene (*.scene)");
	if (path.isEmpty())
	{
		return;
	}
	std::ifstream fin(path.toStdString());
	FishEditor::SceneInputArchive archive(fin);
	//auto gameObjects = archive.LoadAll();
	archive.LoadAll();
}

void MainWindow::SaveSceneAs()
{
	//options |= QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString path = QString::fromStdString(FishEngine::Application::dataPath().string());
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
		archive.BeginMap(3);
		archive << "fileFormatVersion" << 2;
		archive << "guid" << "xxxxxxxx";
		archive << make_nvp("timeCreated", time_created);
		archive.EndMap();
	}
}

void MainWindow::ShowSceneViewDrawModeMenu()
{
	if (m_sceneViewDrawModeMenu == nullptr)
	{
		QAction * action;
		QActionGroup* action_group = new QActionGroup( this );
		m_sceneViewDrawModeMenu = new QMenu(this);
		action = m_sceneViewDrawModeMenu->addAction("Shaded");
		action->setChecked(true);
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action = m_sceneViewDrawModeMenu->addAction("Wireframe");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		//action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Shaded Wireframe");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		//action->setEnabled(false);
		m_sceneViewDrawModeMenu->addSeparator();
		action = m_sceneViewDrawModeMenu->addAction("Shadow Cascades");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		//action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Render Paths");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Alpha Channel");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Overdraw");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Mipmaps");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		m_sceneViewDrawModeMenu->addSeparator();
		action = m_sceneViewDrawModeMenu->addAction("Albedo");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Specular");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Smoothness");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
		action = m_sceneViewDrawModeMenu->addAction("Normal");
		action->setCheckable(true);
		action->setActionGroup(action_group);
		action->setEnabled(false);
//		m_sceneViewDrawModeMenu->addSeparator();
//		m_sceneViewDrawModeMenu->addAction("UV Charts");
//		m_sceneViewDrawModeMenu->addAction("Wireframe");
//		m_sceneViewDrawModeMenu->addAction("Shaded Wireframe");
//		m_sceneViewDrawModeMenu->addSeparator();
	}
	
	m_sceneViewDrawModeMenu->exec(QCursor::pos());
}
