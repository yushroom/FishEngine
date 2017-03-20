#include "ProjectListView.hpp"

#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <QDropEvent>
#include <QMimeData>
#include <QMenu>
#include <QDesktopServices>
#include <QDir>

#include "ProjectViewFileModel.hpp"

#include <Object.hpp>
#include <GameObject.hpp>
#include <Material.hpp>
#include <Application.hpp>

#include <Debug.hpp>
#include <Scene.hpp>
//#include <Serialization/archives/BinaryOutputArchive.hpp>

#include "AssetDataBase.hpp"
#include "SceneArchive.hpp"

ProjectListView::ProjectListView(QWidget *parent /*= 0*/)
	: QListView(parent)
{
	//setAcceptDrops(true);
	//setDragEnabled(true);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	//setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::EditKeyPressed);

	m_menu = new QMenu(this);
	QAction * action = nullptr;
	QMenu * subMenu = nullptr;
	subMenu = m_menu->addMenu("Create");
	{
		action = subMenu->addAction("Folder");
		connect(action, &QAction::triggered, this, &ProjectListView::MakeDir);
		subMenu->addSeparator();
		action = subMenu->addAction("Script");
		action->setEnabled(false);
		action = subMenu->addAction("Shader");
		action->setEnabled(false);
		subMenu->addSeparator();
		action = subMenu->addAction("Scene");
		action->setEnabled(false);
		action = subMenu->addAction("Prefab");
		action->setEnabled(false);
		subMenu->addSeparator();
		m_actionCreateMaterial = subMenu->addAction("Material");
		//action->setEnabled(false);
		connect(m_actionCreateMaterial, &QAction::triggered, this, &ProjectListView::CreateMaterial);
		//m_actionCreateMaterial->setEnabled(false);
		subMenu->addSeparator();
		action = subMenu->addAction("Animator Controller");
		action->setEnabled(false);
		action = subMenu->addAction("Animation");
		action->setEnabled(false);
		action = subMenu->addAction("Animator Override Controller");
		action->setEnabled(false);
		action = subMenu->addAction("Avatar Mask");
		action->setEnabled(false);
	}
#if FISHENGINE_PLATFORM_WINDOWS
	m_actionShowInExplorer = m_menu->addAction("Show in Explorer");
#else
	m_actionShowInExplorer = m_menu->addAction("Show in Finder");
#endif
	connect(
		m_actionShowInExplorer,
		&QAction::triggered,
		this,
		&ProjectListView::ShowInExplorer);
	//connect(action, &QAction::triggered, []() { CreatePrimitive(PrimitiveType::Cube); });
	m_actionOpen = m_menu->addAction("Open");
	m_actionDelete = m_menu->addAction("Delete");
	m_actionMoveToScene = m_menu->addAction("Instantiate in scene");
	//m_actionMoveToScene->setEnabled(false);
	//action->setEnabled(false);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, &ProjectListView::ShowContexMenu);
}

ProjectListView::~ProjectListView()
{

}

void ProjectListView::ShowContexMenu(const QPoint& pos)
{
	auto item = indexAt(pos);
	auto fileInfo = m_fileModel->fileInfo(item);
	if (item.isValid())
	{
		m_actionDelete->setEnabled(true);
		m_actionOpen->setEnabled(true);
	}
	else
	{
		m_actionDelete->setEnabled(false);
		m_actionOpen->setEnabled(false);
		//this->selectionModel()->clearSelection();
	}

	auto path = fileInfo->path();
	bool isModel = path.extension() == ".fbx" || path.extension() == ".obj";
	m_actionMoveToScene->setEnabled(isModel);

	auto action = m_menu->exec(QCursor::pos());
	if (action == nullptr)
	{
		return;
	}

	if (action == m_actionDelete)
	{
		auto selectedRows = this->selectionModel()->selectedRows();
		std::vector<int> selected;
		selected.reserve(selectedRows.size());
		for (auto const & row : selectedRows)
		{
			//m_fileModel->removeRow(row.row());
			selected.push_back(row.row());
		}
		std::sort(selected.begin(), selected.end(), std::greater<int>());
		// started from the end and iterate back to the beginning
		// see http://stackoverflow.com/questions/3325115/remove-a-list-of-selected-items-in-the-qlistview
		for (int row : selected)
		{
			m_fileModel->RemoveItem(row);
		}
	}
	else if (action == m_actionMoveToScene)
	{
		//auto model = FishEditor::AssetDatabase::LoadAssetAtPath<FishEngine::GameObject>(path);
		//std::stringstream sstream;
		//FishEngine::BinaryOutputArchive archive(sstream);
		//archive << model;
		//FishEngine::Scene::AddGameObject(model);
		auto model = FishEngine::As<FishEngine::GameObject>( FishEditor::AssetDatabase::LoadAssetAtPath(path) );
		FishEngine::Object::Instantiate(model);
	}
}

void ProjectListView::MakeDir()
{
	auto index = m_fileModel->AddItem("New Folder", true);
	this->edit(index);
}

void ProjectListView::CreateMaterial()
{
	//auto index = m_fileModel->AddItem("New Material.mat", false);
	//this->edit(index);
	//auto mat = std::make_shared<FishEngine::Material>();
	auto mat = FishEngine::Material::defaultMaterial();
	auto path = Path(m_fileModel->rootPath().toStdString()) / "New Material.mat";
	std::ofstream fout(path.string());
	FishEditor::SingleObjectOutputArchive archive(fout);
	archive << mat;
}

void ProjectListView::ShowInExplorer()
{
	// TODO show file in explorer
	// see http://blog.csdn.net/icatchyou/article/details/40682107
	auto rootPath = m_fileModel->rootPath();
	QDesktopServices::openUrl(QUrl::fromLocalFile(rootPath));
}

void ProjectListView::InstantiateAsset()
{
	
}

//void ProjectListView::mousePressEvent(QMouseEvent *event)
//{
//	FishEngine::Debug::Log("%s", "ProjectListView::mousePressEvent");
//}

void ProjectListView::dragEnterEvent(QDragEnterEvent *e)
{
    FishEngine::Debug::Log("%s", "ProjectListView::dragEnterEvent");
	if (e->mimeData()->hasUrls())
		e->acceptProposedAction();
}

void ProjectListView::dragMoveEvent(QDragMoveEvent *e)
{
    //FishEngine::Debug::Log("%s", "ProjectListView::dragMoveEvent");
    e->acceptProposedAction();
}

Path getUniqueName(Path const & dir, Path const & filename)
{
	auto dest = dir / filename;
	
	if (!boost::filesystem::exists(dest))
	{
		return dest;
	}
	
	auto stem = filename.stem().string();
	auto ext = filename.extension().string();
	
	int id = 1;
	do {
		dest = dir / (stem + " " + boost::lexical_cast<std::string>(id) + ext);
	} while (boost::filesystem::exists(dest));

	return dest;
}

void ProjectListView::dropEvent(QDropEvent *e)
{
	FishEngine::Debug::Log("%s", "ProjectListView::dropEvent");
	auto mimeData = e->mimeData();

	auto currentDir = Path(m_fileModel->rootPath().toStdString()).remove_trailing_separator().make_preferred();
	
	if (mimeData->hasUrls())
	{
		auto projectRoot = FishEngine::Application::dataPath();
		projectRoot = projectRoot.make_preferred();
		for (auto const & url : mimeData->urls())
		{
			auto src = Path(url.toLocalFile().toStdString());
			src = boost::filesystem::absolute(src).make_preferred();
			bool insideProject = boost::starts_with(src.string(), projectRoot.string());
			if (!insideProject)
			{
				auto dest = getUniqueName(currentDir, src.filename());
				boost::filesystem::copy(src, dest);
				FishEngine::Debug::Log("copy file from: [%s] --> [%s]", src.string().c_str(), dest.string().c_str());
				// TODO: .meta
			}
			else
			{
				bool insideThisDir = (src.parent_path() == currentDir);
				if (insideThisDir)
				{
					// do nothing
				}
				else
				{
					auto dest = getUniqueName(currentDir, src.filename());
					boost::filesystem::rename(src, dest);
					auto srcMeta = Path(src.string() + ".meta");
					if (boost::filesystem::exists(srcMeta))
					{
						auto destMeta = Path(dest.string() + ".meta");
						boost::filesystem::rename(srcMeta, destMeta);
					}
				}
			}
		}
		
//		if (!pathList.empty())
//		{
//			e->acceptProposedAction();
//		}
	}
}
