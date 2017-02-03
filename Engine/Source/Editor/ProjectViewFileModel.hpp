#pragma once

#include <boost/filesystem.hpp>
#include <QAbstractItemModel>
#include <QAbstractListModel>

typedef boost::filesystem::path Path;

//class ProjectViewFileModel;
//class ProjectViewDirModel;

class FileInfo
{
public:
    FileInfo() = default;
    //FileNode(FileNode &&) = default;
    FileInfo(FileInfo const &) = delete;
    FileInfo & operator=(FileInfo const &) = delete;

    bool isDir() const { return m_isDirectory; }

    QString absoluteFilePath() const
    {
        return boost::filesystem::absolute(m_path).string().c_str();
    }

    QString fileName() const
    {
        return m_path.filename().string().c_str();
    }

    int childCount() const
    {
        return m_dirChildren.size() + m_fileChildren.size();
    }

    int subDirCount() const
    {
        return m_dirChildren.size();
    }
	
	const Path & path() const
	{
		return m_path;
	}
	
	FileInfo * parent() const
	{
		return m_parent;
	}

    FileInfo * childAt(int index) const
    {
        assert(index >= 0 && index < childCount());
        if (index < m_dirChildren.size())
            return m_dirChildren[index];
        else
            return m_fileChildren[index - m_dirChildren.size()];
    }

    FileInfo * subDirAt(int index) const
    {
        assert(index >= 0 && index < m_dirChildren.size());
        return m_dirChildren[index];
    }
	
	std::vector<FileInfo*> const & subDirs() const
	{
		return m_dirChildren;
	}

    static void SetAssetRootPath(std::string const & path);
    static FileInfo* assetRoot() { return s_assetRoot; };
    static FileInfo* fileInfo(std::string const & path);

private:
    //friend class ProjectViewFileModel;
    //friend class ProjectViewDirModel;

    void BuildNodeTree(const Path & m_path);

    Path                    m_path;
    FileInfo*               m_parent = nullptr;
    bool                    m_isDirectory = false;
    std::vector<FileInfo*>  m_dirChildren;
    std::vector<FileInfo*>  m_fileChildren;

    static FileInfo *                       s_assetRoot;
    static std::map<std::string, FileInfo*> s_nameToNode;
};



class ProjectViewFileModel : public QAbstractListModel
{
public:
    explicit ProjectViewFileModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex setRootPath(const QString &path);

    FileInfo * fileInfo(const QModelIndex &index) const;

private:
    FileInfo * m_rootNode = nullptr;
};

class ProjectViewDirModel : public QAbstractItemModel
{
public:
    explicit ProjectViewDirModel(QObject *parent = nullptr);

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex setRootPath(const Path &path);
    FileInfo * fileInfo(const QModelIndex &index) const;

private:
    //FileInfo * m_rootNode = nullptr;
};
