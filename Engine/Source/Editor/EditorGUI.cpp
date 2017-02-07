#include "EditorGUI.hpp"

#include <cassert>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <LayerMask.hpp>
#include <ReflectClass.hpp>
#include <Vector3.hpp>

#include <QTreeWidget>
#include <QMenu>

#include "UI/UIComponentHeader.hpp"
#include "UI/UIFloat3.hpp"
#include "UI/UIFloat.hpp"
#include "UI/UIComboBox.hpp"
#include "UI/UISlider.hpp"
#include "UI/UIObjecField.hpp"
#include "UI/UIColor.hpp"
#include "UI/UIBool.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIMaterialHeader.hpp"

#include "Inspector.hpp"
#include "UIDebug.hpp"

using namespace FishEditor;
using namespace FishEngine;


QTreeWidget*        EditorGUI::s_treeWidget         = nullptr;

std::stack<QTreeWidgetItem*> EditorGUI::s_itemStack;
std::stack<int>		EditorGUI::s_itemIndexStack;
QTreeWidgetItem*	EditorGUI::s_currentItem;
QTreeWidgetItem*	EditorGUI::s_currentGroupHeaderItem;
int					EditorGUI::s_currentGroupHeaderItemChildIndex;
bool				EditorGUI::s_expectNewGroup;


template<class T, class... Args>
T* EditorGUI::CheckNextWidget(Args&&... args )
{
    static_assert(std::is_base_of<QWidget, T>::value, "T must be a QWidget");

	if (s_expectNewGroup)
	{
		HideAllChildOfLastItem();
		s_expectNewGroup = false;
	}

    // get the item
    QTreeWidgetItem * item;
    if (s_currentGroupHeaderItem == nullptr) // is top item
    {
        if (s_currentGroupHeaderItemChildIndex < s_treeWidget->topLevelItemCount()) // exists, reuse it
        {
            item = s_treeWidget->topLevelItem(s_currentGroupHeaderItemChildIndex);
            if (item->isHidden())
            {
                LOG;
                item->setHidden(false);
            }
        }
        else
        {
            LOG;
            Debug::LogError("[CheckNextWidget] add new QTreeWidgetItem");
            item = new QTreeWidgetItem;
            s_treeWidget->addTopLevelItem(item);
            item->setExpanded(true);
        }
    }
    else
    {
        if (s_currentGroupHeaderItemChildIndex < s_currentGroupHeaderItem->childCount())  // exists, reuse it
        {
            item = s_currentGroupHeaderItem->child(s_currentGroupHeaderItemChildIndex);
            if (item->isHidden())
            {
                LOG;
                item->setHidden(false);
            }
        }
        else
        {
            LOG;
            item = new QTreeWidgetItem;
			s_currentGroupHeaderItem->addChild(item);
        }
    }

	s_currentGroupHeaderItemChildIndex++;
	s_currentItem = item;
	s_expectNewGroup = (s_currentItem->childCount() > 0);

	if (item == nullptr) abort();

    auto widget = qobject_cast<T*>(s_treeWidget->itemWidget(item, 0));

    if (widget == nullptr)
    {
        LOG;
        Debug::LogWarning("[CheckNextWidget] new widget");
        widget = new T(args...);
        s_treeWidget->setItemWidget(item, 0, widget);
    }
    return widget;
}


ComponentPtr componentToBeDestroyed;

void EditorGUI::Begin()
{
	s_currentGroupHeaderItem = nullptr;
	s_currentGroupHeaderItemChildIndex = 0;
	s_expectNewGroup = false;
}

void EditorGUI::End()
{
	if (!s_itemStack.empty() || !s_itemIndexStack.empty())
		abort();

	if (s_expectNewGroup)
	{
		HideAllChildOfLastItem();
		s_expectNewGroup = false;
	}

    int rowCount = s_treeWidget->topLevelItemCount();
    int componentCount = s_currentGroupHeaderItemChildIndex;
    // hide redundant top item
    for (int i = componentCount; i < rowCount; i++)
    {
        auto item = s_treeWidget->topLevelItem(i);
        if (item->isHidden())
            break;  // do not check the rest of rows
        item->setHidden(true);
        LOG;
        Debug::LogError("[EditorGUI::End]hide %d", i);
    }


}

bool FishEditor::EditorGUI::BeginComponent(std::string const & componentTypeName)
{
	auto header = CheckNextWidget<UIComponentHeader>(componentTypeName);
	header->CheckUpdate(componentTypeName);
	bool expanded = s_currentItem->isExpanded();
	PushGroup();
	return expanded;
}

void FishEditor::EditorGUI::EndComponent()
{
	PopGroup();
}

bool FishEditor::EditorGUI::BeginMaterial(std::string const & materialName)
{
	auto header = CheckNextWidget<UIMaterialHeader>(materialName);
	header->CheckUpdate(materialName);
	bool expanded = s_currentItem->isExpanded();
	PushGroup();
	return expanded;
}

void FishEditor::EditorGUI::EndMaterial()
{
	PopGroup();
}


bool EditorGUI::Button(const std::string &text)
{
    UIButton * button = CheckNextWidget<UIButton>(QString::fromStdString(text));
    return button->CheckClicked();
}

bool EditorGUI::Toggle(const std::string & label, bool *value)
{
    UIBool * toggle = CheckNextWidget<UIBool>(label, *value);
    return toggle->CheckUpdate(label, *value);

}

bool EditorGUI::ColorField(const std::string &label, Color *color)
{
    QColor qcolor(color->r * 255, color->g * 255, color->b * 255);
    UIColor * color_line = CheckNextWidget<UIColor>(label, qcolor);
    bool changed = color_line->CheckUpdate(label, qcolor);
    if (changed)
    {
        constexpr float inv_255 = 1.0f / 255.0f;
        color->r = qcolor.red()   * inv_255;
        color->g = qcolor.green() * inv_255;
        color->b = qcolor.blue()  * inv_255;
        return true;
    }
    return false;
}

bool EditorGUI::EnumPopup(const std::string &label, int *index, const char * const *enumStringArray, int arraySize)
{
    UIComboBox * combo = CheckNextWidget<UIComboBox>(label, *index, enumStringArray, arraySize);
    return combo->CheckUpdate(label, *index, enumStringArray, arraySize);
}

bool EditorGUI::FloatField(const std::string &label, float * v)
{
    UIFloat * float_row = CheckNextWidget<UIFloat>(label, *v);
    return float_row->CheckUpdate(label, *v);
}

bool EditorGUI::FloatField(const std::string &label, float v)
{
    UIFloat * float_row = CheckNextWidget<UIFloat>(label, v);
    return float_row->CheckUpdate(label, v);
}

bool EditorGUI::Slider(const std::string &label, float *value, float leftValue, float rightValue)
{
    UISlider * slider = CheckNextWidget<UISlider>(label, *value, leftValue, rightValue);
    return slider->CheckUpdate(label, *value);
}

bool EditorGUI::Vector3Field(const std::string &label, Vector3 *v)
{
    //return false;
    UIFloat3 * float3 = CheckNextWidget<UIFloat3>(label, v->x, v->y, v->z);
    return float3->CheckUpdate(label, v->x, v->y, v->z);
}


void EditorGUI::PushGroup()
{
	if (s_currentItem == nullptr)
	{
		abort();
	}
	s_itemStack.push(s_currentGroupHeaderItem);
	s_itemIndexStack.push(s_currentGroupHeaderItemChildIndex);
	s_currentGroupHeaderItem = s_currentItem;
	s_currentGroupHeaderItemChildIndex = 0;
	s_expectNewGroup = false;
}

void EditorGUI::PopGroup()
{
	// Hide Redundant Child Items
	HideRedundantChildItemsOfLastGroup();

	if (s_itemStack.empty() || s_itemIndexStack.empty())
	{
		abort();
	}
	//s_currentItem = s_currentGroupHeaderItem;	// ?
	s_currentGroupHeaderItem = s_itemStack.top();
	s_itemStack.pop();
	s_currentGroupHeaderItemChildIndex = s_itemIndexStack.top();
	s_itemIndexStack.pop();
}

bool EditorGUI::ObjectField(const std::string &label, const ObjectPtr &obj)
{
    UIObjecField * objField = CheckNextWidget<UIObjecField>(label, obj->name());
    return objField->CheckUpdate(label, obj->name());
}


std::string EditorGUI::ShowAddComponentMenu()
{
    static QMenu* menu = nullptr;
    if (menu == nullptr)
    {
        menu = new QMenu(s_treeWidget);
        menu->addAction("Rigidbody");
    }

    auto action = menu->exec(QCursor::pos());
    return action->text().toStdString();
}


void FishEditor::EditorGUI::HideRedundantChildItemsOfLastGroup()
{
	if (!s_currentGroupHeaderItem->isExpanded())
	{
		return;
	}
	int rowCount = s_currentGroupHeaderItem->childCount();
	for (int i = s_currentGroupHeaderItemChildIndex; i < rowCount; i++)
	{
		auto item = s_currentGroupHeaderItem->child(i);
		if (item->isHidden())
			break;  // do not check the rest of rows
		item->setHidden(true);
		LOG;
		Debug::LogWarning("[EditorGUI::PopGroup] hide %d", i);
	}
}

void FishEditor::EditorGUI::HideAllChildOfLastItem()
{
	int rowCount = s_currentItem->childCount();
	for (int i = 0; i < rowCount; i++)
	{
		auto item = s_currentItem->child(i);
		if (item->isHidden())
			break;  // do not check the rest of rows
		item->setHidden(true);
		LOG;
		Debug::LogWarning("[EditorGUI::PopGroup] hide %d", i);
	}
}
