#pragma once

#include <QAbstractItemModel>
#include <QAbstractListModel>

#include "FileInfo.hpp"

class ProjectViewFileModel : public QAbstractListModel
{
public:
    explicit ProjectViewFileModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex setRootPath(const QString &path);

    FishEditor::FileInfo * fileInfo(const QModelIndex &index) const;

private:
    FishEditor::FileInfo * m_rootNode = nullptr;
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
    FishEditor::FileInfo * fileInfo(const QModelIndex &index) const;
};
