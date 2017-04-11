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

void ObjectListModel::SetObjectType(int classID)
{
	m_cachedObjects.clear();
	for (auto const & pair : AssetImporter::s_importerGUIDToObject)
	{
		if (FishEngine::IsDerivedFrom(pair.second->ClassID(), classID))
		{
			m_cachedObjects.push_back(pair.second);
		}
	}
}

std::shared_ptr<Object> ObjectListModel::object(const QModelIndex &index) const
{
	int row = index.row();
	if (row >= 0 && row < m_cachedObjects.size())
		return m_cachedObjects[index.row()];
	return nullptr;
}

int ObjectListModel::rowCount(const QModelIndex &) const
{
	return m_cachedObjects.size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.model() != this)
		return QVariant();
	int row = index.row();
	if (row >= rowCount() || row < 0)
		return QVariant();

	auto tex = this->object(index);
	auto path = AssetDatabase::GetAssetPath(tex->GetInstanceID());

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
