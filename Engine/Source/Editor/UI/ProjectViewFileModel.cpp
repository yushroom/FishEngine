#include "ProjectViewFileModel.hpp"

#include <QIcon>
#include <Debug.hpp>
#include "AssetDataBase.hpp"

using namespace FishEngine;
using namespace FishEditor;


ProjectViewFileModel::ProjectViewFileModel(QObject *parent)
	: QAbstractListModel(parent)
{
}


int ProjectViewFileModel::rowCount(const QModelIndex &) const
{
	if (m_rootNode == nullptr)
		return 0;
	return static_cast<int>( m_rootNode->childCount() );
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
		return FishEditor::AssetDatabase::GetCacheIcon(node->absoluteFilePath());
		break;
	}

	return QVariant();
}


Qt::ItemFlags ProjectViewFileModel::flags(const QModelIndex &index) const
{
	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}


bool ProjectViewFileModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
	LogWarning("ProjectViewFileModel::setData");
	//return true;
	if (!index.isValid())
	{
		return false;
	}

	if (role == Qt::EditRole)
	{
		auto fi = this->fileInfo(index);
		fi->Rename(value.toString().toStdString());
		return true;
	}
	
	return false;
}


//bool ProjectViewFileModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
//{
//	beginRemoveRows(parent, row, row + count - 1); // note "-1"
//	for (int i = 0; i < count; ++i)
//	{
//		m_rootNode->m_dirChildren[i]->m_fileExists = false;
//		boost::system::error_code error;
//		boost::filesystem::remove(m_rootNode->m_dirChildren[i]->m_path, error);
//		if (error)
//		{
//			abort();
//		}
//	}
//	auto begin = m_rootNode->m_dirChildren.begin() + row;
//	m_rootNode->m_dirChildren.erase(begin, begin + count);
//	endRemoveRows();
//	return true;
//}

QString ProjectViewFileModel::rootPath() const
{
	return QString::fromStdString( m_rootNode->path().string() );
}


QModelIndex ProjectViewFileModel::setRootPath(const QString &path)
{
	auto p = boost::filesystem::absolute(path.toStdString()).make_preferred().string();
	m_rootNode = FileInfo::fileInfo(p);

	//FishEngine::Debug::LogError("ProjectViewFileModel::setRootPath: %s", path.toStdString().c_str());
	return QModelIndex();
}


QModelIndex ProjectViewFileModel::AddItem(QString const & name, bool isDir)
{
	int row = static_cast<int>(m_rootNode->subDirCount());
	beginInsertRows(QModelIndex(), row, row);
	if (!isDir)
	{
		// not implemented
		abort();
	}
	auto fileInfo = m_rootNode->CreateNewSubDir(name.toStdString());
	fileInfo->m_isDirectory = true;
	endInsertRows();
	return createIndex(row, 0, fileInfo);
}


void ProjectViewFileModel::RemoveItem(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	auto node = m_rootNode->childAt(row);
	node->DeleteFile();

	if (node->isDir())
	{
		auto iter = m_rootNode->m_dirChildren.begin() + row;
		m_rootNode->m_dirChildren.erase(iter);
	}
	else
	{
		auto iter = m_rootNode->m_fileChildren.begin() + (row - m_rootNode->subDirCount());
		m_rootNode->m_fileChildren.erase(iter);
	}

	endRemoveRows();
}

FileInfo *ProjectViewFileModel::fileInfo(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return m_rootNode;
	}
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
		row = parentNode->parent()->GetChildIndex(parentNode);
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
	return static_cast<int>( parentNode->subDirCount() );
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
		return QIcon(":/Resources/Assets/Folder@64.png");
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
		row = node->parent()->GetChildIndex(node);
	}

	return createIndex(row, 0, node);
}


FileInfo *ProjectViewDirModel::fileInfo(const QModelIndex &index) const
{
	if (index.isValid())
		return static_cast<FileInfo*>(index.internalPointer());
	return nullptr;
}
