#include "EditorGUI.hpp"

#include <cassert>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <LayerMask.hpp>
#include <ReflectClass.hpp>
#include <Vector3.hpp>
#include <Texture.hpp>

#include <QTreeWidget>
#include <QMenu>

#include "UI/UIComponentHeader.hpp"
#include "UI/UIFloat3.hpp"
#include "UI/UIFloat4.hpp"
#include "UI/UIFloat.hpp"
#include "UI/UIComboBox.hpp"
#include "UI/UISlider.hpp"
#include "UI/UIObjecField.hpp"
#include "UI/UIColor.hpp"
#include "UI/UIBool.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIMaterialHeader.hpp"
#include "UI/UITexture.hpp"
#include "UI/SelectObjectDialog.hpp"

#include "Inspector.hpp"
#include "UI/UIDebug.hpp"

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
			LogInfo("[CheckNextWidget] add new QTreeWidgetItem");
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
		LogInfo("[CheckNextWidget] new widget");
		widget = new T(args...);
		s_treeWidget->setItemWidget(item, 0, widget);
	}
	return widget;
}


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
		LogInfo(Format( "hide %1%", i ));
	}
}

bool EditorGUI::BeginComponent(std::string const & componentTypeName, UIHeaderState * outState)
{
	auto header = CheckNextWidget<UIComponentHeader>(componentTypeName);
	*outState = header->CheckUpdate(componentTypeName);
	bool expanded = s_currentItem->isExpanded();
	PushGroup();
	return expanded;
}

bool EditorGUI::BeginComponent(std::string const & componentTypeName, bool enabled, UIHeaderState * outState)
{
	auto header = CheckNextWidget<UIComponentHeader>(componentTypeName, enabled);
	*outState = header->CheckUpdate(componentTypeName, enabled);
	bool expanded = s_currentItem->isExpanded();
	PushGroup();
	return expanded;
}

void EditorGUI::EndComponent()
{
	PopGroup();
}

bool EditorGUI::BeginMaterial(std::string const & materialName)
{
	auto header = CheckNextWidget<UIMaterialHeader>(materialName);
	header->CheckUpdate(materialName);
	bool expanded = s_currentItem->isExpanded();
	PushGroup();
	return expanded;
}

void EditorGUI::EndMaterial()
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
	UIFloat3 * float3 = CheckNextWidget<UIFloat3>(label, v->x, v->y, v->z);
	return float3->CheckUpdate(label, v->x, v->y, v->z);
}


bool EditorGUI::Vector4Field(std::string const & label, FishEngine::Vector4 * v)
{
	UIFloat4 * float4 = CheckNextWidget<UIFloat4>(label, v->x, v->y, v->z, v->w);
	return float4->CheckUpdate(label, v->x, v->y, v->z, v->w);
}


FishEngine::ObjectPtr EditorGUI::ObjectFieldImpl(std::string const & label, FishEngine::ObjectPtr const & obj)
{
	std::string name = "none";
	if (obj != nullptr)
		name = obj->name();
	UIObjecField * objField = CheckNextWidget<UIObjecField>(label, name);
	auto ret = obj;
	if (objField->CheckUpdate(label, name))
	{
		//LogWarning("Select Object");
		SelectObjectDialog dialog;
		dialog.ShowWithCallback(obj->ClassID(), [&ret](FishEngine::ObjectPtr object) {
			ret = object;
			//LogWarning(FishEngine::Format("Select Object %1%", ret->name().c_str()));
		});
	}
	return ret;
}


bool EditorGUI::TextureField(std::string const & label, FishEngine::TexturePtr & texture)
{
	UITexture * t = CheckNextWidget<UITexture>(label);
	if (texture == nullptr)
	{
		return false;
	}
	bool changed = false;
	UITextureState state = t->CheckUpdate(label, texture->GetInstanceID());
	if (state == UITextureState::TextureClicked)
	{

	}
	else if (state == UITextureState::SelectButtonClicked)
	{
		SelectObjectDialog dialog;
		dialog.ShowWithCallback(FishEngine::ClassID<FishEngine::Texture2D>(), [&texture, &changed](FishEngine::ObjectPtr obj) {
			if (texture->GetInstanceID() != obj->GetInstanceID())
			{
				texture = FishEngine::As<Texture>(obj);
				changed = true;
			}
			//LogError(Format("Select Texture %s", obj->name().c_str()));
		});
	}
	return changed;
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


void EditorGUI::HideRedundantChildItemsOfLastGroup()
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
		//Debug::Log("[EditorGUI::PopGroup] hide %d", i);
	}
}

void EditorGUI::HideAllChildOfLastItem()
{
	int rowCount = s_currentItem->childCount();
	for (int i = 0; i < rowCount; i++)
	{
		auto item = s_currentItem->child(i);
		if (item->isHidden())
			break;  // do not check the rest of rows
		item->setHidden(true);
		LOG;
		//Debug::Log("[EditorGUI::PopGroup] hide %d", i);
	}
}
