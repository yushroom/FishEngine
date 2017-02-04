#pragma once

#include <QAbstractListModel>

class ObjectListModel : public QAbstractListModel
{
public:
    explicit ObjectListModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};
