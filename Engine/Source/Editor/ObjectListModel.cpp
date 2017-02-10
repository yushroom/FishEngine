#include "ObjectListModel.hpp"

#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"

#include <Texture.hpp>
#include <Debug.hpp>

using namespace FishEngine;
using namespace FishEditor;

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    //Debug::LogError("ObjectListModel::ObjectListModel");
}

std::shared_ptr<Object> ObjectListModel::object(const QModelIndex &index) const
{
    auto it = AssetImporter::s_importerGuidToTexture.begin();
    for (int i = 0; i < index.row(); ++i)
        it++;
    return it->second;
}

int ObjectListModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>( AssetImporter::s_importerGuidToTexture.size() );
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.model() != this)
		return QVariant();
	int row = index.row();
	if (row >= rowCount() || row < 0)
		return QVariant();

    auto tex = this->object(index);
	auto path = AssetDatabase::GUIDToAssetPath(tex->GetGUID());

	switch (role)
	{
	case Qt::EditRole:
	case Qt::DisplayRole:
		return QString::fromStdString(path.stem().string());
		break;
	case Qt::DecorationRole:
		return FishEditor::AssetDatabase::GetCacheIcon(boost::filesystem::absolute(path).string());
		break;
	}

    return QVariant();
}
