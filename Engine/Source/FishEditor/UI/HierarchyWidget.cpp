#include "HierarchyWidget.hpp"
#include <FishGUI/Draw.hpp>
#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Theme.hpp>
#include <FishGUI/Input.hpp>
#include <FishEngine/Scene.hpp>

//#include <algorithm>

using namespace FishGUI;
using namespace FishEngine;

HierarchyWidget::HierarchyWidget(const char* name) : Super(name)
{
	m_model = new HierarchyModel();
	m_imContext->ymargin = 0;
	m_unfolded.insert(m_root);
}

