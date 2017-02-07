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

    dirModel = new ProjectViewDirModel(this);
    //dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->dirTreeView->setModel(dirModel);

//    for (int i = 1; i < tree->header()->count(); ++i)
//    {
//        tree->hideColumn(i);
//    }
//    tree->header()->hide();

    fileModel = new ProjectViewFileModel(this);
    ui->listView->setModel(fileModel);
    ui->iconSizeSlider->setMinimum(0);
    ui->iconSizeSlider->setMaximum(32);
    ui->iconSizeSlider->setValue(m_listViewIconSize);
    OnIconSizeChanged(m_listViewIconSize);

//    QDir cwd = QCoreApplication::applicationDirPath();
//    cwd.cdUp();
//    cwd.cdUp();

    //SetRootPath(cwd.absolutePath());
    //const char * rootPath = "/Users/yushroom/program/graphics/FishEngine/Example/Sponza";
    auto const & rootPath = FishEngine::Applicaiton::dataPath();
    SetRootPath(QString::fromStdString(rootPath));

    connect(ui->dirTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ProjectView::OnDirTreeViewSelectionChanged);
    connect(ui->listView, &QListView::doubleClicked, this, &ProjectView::OnListTreeViewDoubleClicked);
	//connect(ui->listView, &QListView::clicked, this, &ProjectView::OnListTreeViewClicked);
    connect(ui->listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ProjectView::OnListTreeViewSelectionChanged);
    connect(ui->iconSizeSlider, &QSlider::valueChanged, this, &ProjectView::OnIconSizeChanged);
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
    //auto dir = QDir::cleanPath(path);
    //Debug::LogError("ProjectView::SetRootPath %s", path.toStdString().c_str());
    //ui->dirTreeView->setRootIndex(dirModel->setRootPath(path));
//    const auto & rootIndex = dirModel->index(dir);
//    if (rootIndex.isValid())
//        ui->dirTreeView->setRootIndex(rootIndex);
//    fileModel->setRootPath(dir);
//    ui->listView->setRootIndex(fileModel->setRootPath(dir));
    ui->listView->setRootIndex(fileModel->setRootPath(path));
    //ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(path));
}

void ProjectView::OnDirTreeViewSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
    auto path = QString::fromStdString(dirModel->fileInfo(current)->absoluteFilePath().string());
    ui->listView->setRootIndex(fileModel->setRootPath(path));
}


void ProjectView::OnListTreeViewDoubleClicked(const QModelIndex &index)
{
    auto info = fileModel->fileInfo(index);
    if (info->isDir())
    {
        auto path = QString::fromStdString(info->absoluteFilePath().string());
        ui->listView->setRootIndex(fileModel->setRootPath(path));
		ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(info->path()));
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

void ProjectView::OnListTreeViewSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
    auto info = fileModel->fileInfo(current);
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
