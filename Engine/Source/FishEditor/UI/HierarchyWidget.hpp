#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Scene.hpp>

#include <vector>
#include <set>
#include <FishGUI/Widget.hpp>
#include <FishGUI/ModelView/ItemView.hpp>
#include <FishGUI/ModelView/TreeWidget.hpp>

//using FishEngine::Transform;
//using FishEngine::TransformPtr;

//typedef TTreeModel<TransformPtr> HierarchyModel;

/*
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
inline std::string HierarchyModel::text(TransformPtr item) const
{
	return item->name();
}
*/

struct HierarchyModel : public FishGUI::TItemModel<FishEngine::Transform*>
{
	typedef FishEngine::Transform* T;
public:
	
	virtual T Parent(T child) const override
	{
		return child->parent().get();
	}

	virtual T ChildAt(int index, T parent) const override
	{
		if (parent == nullptr)
		{
			auto it = FishEngine::Scene::GameObjects().begin();
			std::advance(it, index);
			return (*it)->transform().get();
		}
		else
		{
			auto it = parent->children().begin();
			std::advance(it, index);
			return (*it).get();
		}
	}

	virtual int ChildCount(T parent) const override
	{
		if (parent == nullptr)
			return FishEngine::Scene::GameObjects().size();
		else
			return parent->children().size();
	}

	virtual std::string Text(T item) const override
	{
		return item->name();
	}
};


class HierarchyWidget : public FishGUI::TreeWidget<FishEngine::Transform*>
{
public:
	typedef FishGUI::TreeWidget<FishEngine::Transform*> Super;
	
	HierarchyWidget(const char* name);
	
private:
};
