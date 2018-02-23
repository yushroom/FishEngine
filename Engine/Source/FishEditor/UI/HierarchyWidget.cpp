#include "HierarchyWidget.hpp"
#include <FishGUI/Draw.hpp>
#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Theme.hpp>
#include <FishGUI/Input.hpp>
#include <FishEngine/Scene.hpp>

#include "../Selection.hpp"

//#include <algorithm>

using namespace FishGUI;
using namespace FishEngine;


struct HierarchyModel : public FishGUI::TItemModel<TransformPtr>
{
	typedef TransformPtr T;
public:
	
	virtual T Parent(T child) const override
	{
		return child->parent();
	}
	
	virtual T ChildAt(int index, T parent) const override
	{
		if (parent == nullptr)
		{
			auto it = FishEngine::Scene::GameObjects().begin();
			std::advance(it, index);
			return (*it)->transform();
		}
		else
		{
			auto it = parent->children().begin();
			std::advance(it, index);
			return *it;
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


HierarchyWidget::HierarchyWidget(const char* name) : Super(name)
{
	m_model = new HierarchyModel();
	m_imContext->ymargin = 0;
	m_unfolded.insert(m_root);
	
	m_selectionModel.selectionChanged.connect([this](TransformPtr selected){
		this->OnHierarchyViewSelectionChanged();
	});
}

void HierarchyWidget::OnHierarchyViewSelectionChanged()
{
//	puts("HierarchyWidget::OnHierarchyViewSelectionChanged");
	if (m_selectionModel.SelectedItems().empty())
	{
		FishEditor::Selection::setTransforms({});
	}
	else
	{
		std::list<std::weak_ptr<FishEngine::Transform>> selection;
		for (auto& s : m_selectionModel.SelectedItems())
		{
			selection.push_back(s);
		}
		FishEditor::Selection::setTransforms(selection);
	}
}
