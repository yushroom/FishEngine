#include "ProjectView.hpp"
#include "ui_ProjectView.h"

//#include "ProjectFileIconProvider.hpp"
#include "ProjectViewFileModel.hpp"
//#include <QDirModel>
#include <QDir>

#include <Debug.hpp>
#include <Application.hpp>
#include <AssetImporter.hpp>
#include <TextureImporter.hpp>
#include "Selection.hpp"

using namespace FishEngine;
using namespace FishEditor;

ProjectView::ProjectView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectView)
{
    ui->setupUi(this);

	ui->listView->setResizeMode(QListView::Adjust);
	ui->listView->setDragEnabled(true);
	ui->listView->viewport()->setAcceptDrops(true);
	ui->listView->setDropIndicatorShown(true);
	

    m_dirModel = new ProjectViewDirModel(this);
    //dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->dirTreeView->setModel(m_dirModel);

//    for (int i = 1; i < tree->header()->count(); ++i)
//    {
//        tree->hideColumn(i);
//    }
//    tree->header()->hide();

    m_fileModel = new ProjectViewFileModel(this);
	ui->listView->setFileModel(m_fileModel);
    ui->listView->setModel(m_fileModel);
    ui->iconSizeSlider->setMinimum(0);
    ui->iconSizeSlider->setMaximum(32);
    ui->iconSizeSlider->setValue(m_listViewIconSize);
    OnIconSizeChanged(m_listViewIconSize);

    auto const & rootPath = FishEngine::Applicaiton::dataPath();
    SetRootPath(QString::fromStdString(rootPath.string()));

    connect(
		ui->dirTreeView->selectionModel(), 
		&QItemSelectionModel::currentChanged, 
		this, 
		&ProjectView::OnDirTreeViewSelectionChanged);

    connect(
		ui->listView, 
		&QListView::doubleClicked, 
		this,
		&ProjectView::OnListViewDoubleClicked);

	//connect(ui->listView, &QListView::clicked, this, &ProjectView::OnListTreeViewClicked);
    connect(
		ui->listView->selectionModel(),
		&QItemSelectionModel::currentChanged, 
		this, 
		&ProjectView::OnListViewSelectionChanged);

    connect(
		ui->iconSizeSlider, 
		&QSlider::valueChanged, 
		this, 
		&ProjectView::OnIconSizeChanged);

	//connect(m_fileModel, &ProjectViewFileModel::rowsRemoved, m_dirModel, &ProjectViewDirModel::)
}

ProjectView::~ProjectView()
{
    delete ui;
}

QSize ProjectView::sizeHint() const
{
    return QSize(400, 200);
}

void ProjectView::SetRootPath(const std::string &path)
{
    SetRootPath(QString::fromStdString(path));
}

void ProjectView::SetRootPath(const QString &path)
{
	if (path.isEmpty() || path.isNull())
		return;
    ui->listView->setRootIndex(m_fileModel->setRootPath(path));
	ui->listView->selectionModel()->clearSelection();
    //ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(path));
}

void ProjectView::OnDirTreeViewSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
	if (!current.isValid())
	{
		return;
	}
    auto path = QString::fromStdString(m_dirModel->fileInfo(current)->absoluteFilePath().string());
	if (path.isEmpty() || path.isNull())
		return;
    ui->listView->setRootIndex(m_fileModel->setRootPath(path));
	ui->listView->selectionModel()->clearSelection();
}


void ProjectView::OnListViewDoubleClicked(const QModelIndex &index)
{
    auto info = m_fileModel->fileInfo(index);
    if (info->isDir())
    {
        auto path = QString::fromStdString(info->absoluteFilePath().string());
        ui->listView->setRootIndex(m_fileModel->setRootPath(path));
		ui->listView->selectionModel()->clearSelection();
		ui->dirTreeView->setCurrentIndex(m_dirModel->setRootPath(info->path()));
    }
}

//void ProjectView::OnListTreeViewClicked(const QModelIndex &index)
//{
//	auto info = fileModel->fileInfo(index);
//	auto path = boost::filesystem::absolute(info->path());
//	//auto ext = path.extension();
//	//if (ext == ".png" || ext == ".jpg")
//	//{
//	//	//auto importer = AssetImporter::GetAtPath(path);
//	//	//Selection::setActiveObject(importer);
//	//	auto importer = std::make_shared<TextureImporter>();
//	//	importer->FromFile(path);
//	//}
//	auto importer = AssetImporter::GetAtPath(path);
//	Selection::setTransforms({});
//	Selection::setActiveObject(importer);
//}

void ProjectView::OnListViewSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
    auto info = m_fileModel->fileInfo(current);
    ui->fileNameLabel->setText(QString::fromStdString(info->fileName()));
	auto path = boost::filesystem::absolute(info->path());
	auto importer = AssetImporter::GetAtPath(path);
	Selection::setTransforms({});
	Selection::setActiveObject(importer);
}

void ProjectView::OnIconSizeChanged(int size)
{
    m_listViewIconSize = size;
    if (size < 4)
    {
        ui->listView->setViewMode(QListView::ListMode);
        //ui->listView->setSpacing(0);
        ui->listView->setGridSize(QSize());
        ui->listView->setIconSize(QSize(12, 12));
    }
    else
    {
        ui->listView->setViewMode(QListView::IconMode);
        //ui->listView->setSpacing(-1);
        int s = size * 4;
        ui->listView->setGridSize(QSize(s+12, s+24));
        ui->listView->setIconSize(QSize(s, s));
    }
}
