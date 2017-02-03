#include "ProjectViewFileModel.hpp"

#include <QIcon>
#include <Debug.hpp>

std::map<std::string, FileInfo*> FileInfo::s_nameToNode;
FileInfo * FileInfo::s_assetRoot = nullptr;

//enum class AssetType
//{
//    Unknown,
//    Texture,
//    Model,
//    Shader,
//    Material,
//    Script,
//};

//AssetType GetAssetType(Path const & ext)
//{
//    //auto ext = path.extension();
//    if (ext == ".jpg" || ext == ".png")
//    {
//        return AssetType::Texture;
//    }
//    else if (ext == ".obj" || ext == ".fbx")
//    {
//        return AssetType::Model;
//    }
//    else if (ext == ".shader")
//    {
//        return AssetType::Shader;
//    }
//    else if (ext == ".mat")
//    {
//        return AssetType::Material;
//    }
//    else if (ext == ".hpp" || ext == ".cpp")
//    {
//        return AssetType::Script;
//    }
//    return AssetType::Unknown;
//}


void FileInfo::SetAssetRootPath(const std::string &path)
{
    assert(boost::filesystem::is_directory(path));
    s_assetRoot = new FileInfo();
    s_assetRoot->m_isDirectory = true;
    s_assetRoot->m_parent = nullptr;
    s_assetRoot->m_path = path;
    //s_nameToNode[boost::filesystem::absolute(path).string()] = this;
    s_assetRoot->BuildNodeTree(path);
}

FileInfo* FileInfo::fileInfo(const std::string &path)
{
    auto const & it = s_nameToNode.find(path);
    if (it == FileInfo::s_nameToNode.end())
    {
        abort();
    }
    return it->second;
}


// path must be a dir
void FileInfo::BuildNodeTree(const Path &path)
{
    s_nameToNode[boost::filesystem::absolute(path).string()] = this;
    for (auto& it : boost::filesystem::directory_iterator(path))
    {
        auto & p = it.path();
        if (p.extension() == ".DS_Store" || p.extension() == ".meta")
        {
            continue;
        }

        auto fileNode = new FileInfo();
        fileNode->m_path = p;
        fileNode->m_parent = this;

        if (boost::filesystem::is_directory(p))
        {
            m_dirChildren.emplace_back(fileNode);
            fileNode->m_isDirectory = true;
            fileNode->BuildNodeTree(p);
        }
        else
        {
            m_fileChildren.emplace_back(fileNode);
            fileNode->m_isDirectory = false;
        }
    }
}


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
        return node->fileName();
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

    FishEngine::Debug::LogError("ProjectViewFileModel::setRootPath: %s", path.toStdString().c_str());
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
        return node->fileName();
        break;
    case Qt::DecorationRole:
        return QIcon(":/Resources/folder.png");
        break;
    }

    return QVariant();
}

QModelIndex ProjectViewDirModel::setRootPath(const Path &path)
{
    FishEngine::Debug::LogError("ProjectViewDirModel::setRootPath: %s", path.string().c_str());
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
