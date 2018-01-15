#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Scene.hpp>

#include <vector>
#include <set>
#include <FishGUI/Widget.hpp>
#include <TSelectionModel.hpp>
#include <ItemModel.hpp>
#include <TreeViewWidget.hpp>


using FishEngine::TransformPtr;

//struct GameObject
//{
//	std::string name;
//	std::vector<GameObject*> children;
//
//	GameObject(const std::string& name) : name(name) {}
//};

typedef TTreeModel<TransformPtr, std::string> HierarchyModel;

template<>
inline TransformPtr HierarchyModel::childAt(TransformPtr parent, int row) const
{
	if (parent == m_root)
	{
		auto it = FishEngine::Scene::GameObjects().begin();
		std::advance(it, row);
		return (*it)->transform();
	}
	else
	{
		auto it = parent->children().begin();
		std::advance(it, row);
		return *it;
	}
}

template<>
inline int HierarchyModel::rowCount(TransformPtr parent) const
{
	if (parent == nullptr)
		return 0;
	if (parent == m_root)
		return FishEngine::Scene::GameObjects().size();
	else
		return parent->children().size();
}


template<>
inline std::string HierarchyModel::data(TransformPtr item) const
{
	return item->name();
}


//struct HierarchyModel : public AbstractTreeModel<GameObject, std::string>
//{
//public:
//
//	virtual GameObject* childAt(GameObject* parent, int row) const override
//	{
//		return parent->children[row];
//	}
//
//	virtual int rowCount(GameObject* parent) const override
//	{
//		if (parent == nullptr)
//			return 0;
//		return parent->children.size();
//	}
//
//	virtual bool hasChildren(GameObject* parent) const override
//	{
//		if (parent == nullptr)
//			return false;
//		return !parent->children.empty();
//	}
//
//	virtual std::string data(GameObject* item) const override
//	{
//		return item->name;
//	}
//};


class HierarchyWidget : public FishGUI::TreeWidget<TransformPtr>
{
public:
	typedef FishGUI::TreeWidget<TransformPtr> Super;
	
	HierarchyWidget(const char* name);
	
private:
};
