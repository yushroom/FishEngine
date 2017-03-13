#pragma once

#include <QAbstractItemModel>
#include <QAbstractListModel>

#include "FileInfo.hpp"

class ProjectViewFileModel : public QAbstractListModel
{
public:
	explicit ProjectViewFileModel(QObject *parent = nullptr);

	virtual int				rowCount(const QModelIndex & parent = QModelIndex()) const override;
	virtual QVariant		data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	virtual Qt::ItemFlags	flags(const QModelIndex &index) const override;
	virtual bool			setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	//virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	QString rootPath() const;
	QModelIndex setRootPath(const QString &path);

	QModelIndex AddItem(QString const & name, bool isDir);
	void RemoveItem(int row);

	FishEditor::FileInfo * fileInfo(const QModelIndex &index) const;

Q_SIGNALS:
	void DirRemoved();
	void DirAdded();

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
