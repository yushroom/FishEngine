#pragma once

#include <QAbstractListModel>
#include <memory>

namespace FishEngine
{
    class Object;
}

class ObjectListModel : public QAbstractListModel
{
public:
    explicit ObjectListModel(QObject *parent = nullptr);

	void SetModelType();

    std::shared_ptr<FishEngine::Object> object(const QModelIndex &index) const;

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};
