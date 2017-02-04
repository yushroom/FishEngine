#include "ProjectViewFileModel.hpp"

#include <QIcon>
#include <Debug.hpp>

using namespace FishEngine;
using namespace FishEditor;


ProjectViewFileModel::ProjectViewFileModel(QObject *parent)
    : QAbstractListModel(parent)
{
}


int ProjectViewFileModel::rowCount(const QModelIndex &) const
{
	return m_rootNode->childCount();
}


QVariant ProjectViewFileModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.model() != this )
        return QVariant();
    int row = index.row();
    if ( row >= m_rootNode->childCount() || row < 0 )
        return QVariant();

    auto const & node = m_rootNode->childAt(index.row());
	
    switch (role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return QString::fromStdString(node->stem());
        break;
    case Qt::DecorationRole:
        if (node->isDir())
            return QIcon(":/Resources/folder.png");
        return QIcon(":/Resources/unknown_file.png");
        break;
    }

    return QVariant();
}


QModelIndex ProjectViewFileModel::setRootPath(const QString &path)
{
    auto p = boost::filesystem::absolute(path.toStdString()).string();
    m_rootNode = FileInfo::fileInfo(p);

    //FishEngine::Debug::LogError("ProjectViewFileModel::setRootPath: %s", path.toStdString().c_str());
    return QModelIndex();
}


FileInfo *ProjectViewFileModel::fileInfo(const QModelIndex &index) const
{
    return m_rootNode->childAt(index.row());
}




ProjectViewDirModel::ProjectViewDirModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}


QModelIndex ProjectViewDirModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
        return createIndex(row, column, FileInfo::assetRoot());

    FileInfo * parentNode = static_cast<FileInfo*>(parent.internalPointer());
    FileInfo * childNode = parentNode->subDirAt(row);
    assert(childNode != nullptr);
    return createIndex(row, column, childNode);
}


QModelIndex ProjectViewDirModel::parent(const QModelIndex &child) const
{
    if ( !child.isValid() )
        return QModelIndex();
    auto node = static_cast<FileInfo*>(child.internalPointer());
	auto parentNode = node ? node->parent() : nullptr;
	if (parentNode == nullptr)
		return QModelIndex();
	
	// get the parent's row
	int row = 0;
	if (parentNode->parent() != nullptr)
	{
        auto const & children = parentNode->parent()->subDirs();
		row = std::distance(std::find(children.begin(), children.end(), parentNode), children.begin());
	}
	
	return createIndex(row, 0, parentNode);
}


int ProjectViewDirModel::rowCount(const QModelIndex &parent) const
{
    // root
    if ( !parent.isValid() )
    {
        return 1;
    }
    auto parentNode = static_cast<FileInfo*>(parent.internalPointer());
    return parentNode->subDirCount();
}


int ProjectViewDirModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}


QVariant ProjectViewDirModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.model() != this )
        return QVariant();
//    int row = index.row();
//    if ( row >= m_rootNode->childCount() || row < 0 )
//        return QVariant();

    auto node = fileInfo(index);
    assert(node->isDir());

    switch (role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return QString::fromStdString(node->fileName());
        break;
    case Qt::DecorationRole:
        return QIcon(":/Resources/folder.png");
        break;
    }

    return QVariant();
}

QModelIndex ProjectViewDirModel::setRootPath(const Path &path)
{
    //FishEngine::Debug::LogError("ProjectViewDirModel::setRootPath: %s", path.string().c_str());
    auto p = boost::filesystem::absolute(path).string();
    auto node = FileInfo::fileInfo(path.string());

    int row = 0;

    if (node->parent() != nullptr)
    {
        auto const & children = node->parent()->subDirs();
        row = std::distance(std::find(children.begin(), children.end(), node), children.begin());
    }

    return createIndex(row, 0, node);
}

FileInfo *ProjectViewDirModel::fileInfo(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<FileInfo*>(index.internalPointer());
    return nullptr;
}
