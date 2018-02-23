#pragma once

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Scene.hpp>

#include <vector>
#include <set>
#include <FishGUI/Widget.hpp>
#include <FishGUI/ModelView/ItemView.hpp>
#include <FishGUI/ModelView/TreeWidget.hpp>

typedef std::shared_ptr<FishEngine::Transform> TransformPtr;


class HierarchyWidget : public FishGUI::TreeWidget<TransformPtr>
{
public:
	typedef FishGUI::TreeWidget<TransformPtr> Super;
	
	HierarchyWidget(const char* name);
	
private:
	void OnHierarchyViewSelectionChanged();
};
