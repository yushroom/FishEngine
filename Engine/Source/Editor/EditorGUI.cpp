#include "EditorGUI.hpp"

#include <cassert>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <LayerMask.hpp>
#include <ReflectClass.hpp>
#include <Vector3.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <MeshFilter.hpp>
#include <Mesh.hpp>
#include <MeshRenderer.hpp>
#include <CameraController.hpp>
#include <Rigidbody.hpp>
#include <Collider.hpp>
#include <BoxCollider.hpp>
#include <SphereCollider.hpp>
#include <Shader.hpp>
#include <Material.hpp>

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


#include "Inspector.hpp"

using namespace FishEditor;
using namespace FishEngine;

#include "UIDebug.hpp"

int                 EditorGUI::s_indentLevel        = 0;
int                 EditorGUI::s_topLevelItemIndex  = 0;
int                 EditorGUI::s_localItemIndex     = 0;
QTreeWidget*        EditorGUI::s_treeWidget         = nullptr;
bool                EditorGUI::s_currentHeaderItemIsExpanded = false;
QTreeWidgetItem*    EditorGUI::s_currentHeaderItem  = nullptr;

template<class T, class... Args>
T* EditorGUI::CheckNextWidget(Args&&... args )
{
    static_assert(std::is_base_of<QWidget, T>::value, "T must be a QWidget");

    QTreeWidgetItem * item;
    if (s_localItemIndex < s_currentHeaderItem->childCount())  // exists, reuse it
    {
        item = s_currentHeaderItem->child(s_localItemIndex);
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
        s_currentHeaderItem->addChild(item);
    }
    s_localItemIndex ++;

    auto widget = qobject_cast<T*>(s_treeWidget->itemWidget(item, 0));

    if (widget == nullptr)
    {
        if (item == nullptr)
        {
            LOG;
            Debug::LogWarning("[CheckNextWidget] new QTreeWidgetItem");
            item = new QTreeWidgetItem;
            s_treeWidget->addTopLevelItem(item); // TODO
        }
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
    s_topLevelItemIndex = 0;
    s_localItemIndex = 0;
    s_currentHeaderItem = nullptr;
    s_currentHeaderItemIsExpanded = false;
}

void EditorGUI::End()
{
    HideRedundantChildItems();
    int rowCount = s_treeWidget->topLevelItemCount();
    int componentCount = s_topLevelItemIndex;
    // hide redundant item
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


bool FishEditor::EditorGUI::Foldout(const std::string &name, bool &enabled, UIHeaderState &state)
{
    HideRedundantChildItems();

    QTreeWidgetItem * item;
    bool expanded = true;
    if (s_topLevelItemIndex < s_treeWidget->topLevelItemCount()) // exists, reuse it
    {
        item = s_treeWidget->topLevelItem(s_topLevelItemIndex);
        if (item->isHidden())
        {
            Debug::LogWarning("[Foldout] hide");
            LOG;
            item->setHidden(false);
        }
        expanded = item->isExpanded();
        UIComponentHeader * header = qobject_cast<UIComponentHeader*>(s_treeWidget->itemWidget(item, 0));
        assert(header != nullptr);
        state = header->CheckUpdate(name, enabled);
    }
    else
    {
        Debug::LogError("[Foldout] add new QTreeWidgetItem");
        item = new QTreeWidgetItem;
        s_treeWidget->addTopLevelItem(item);
        UIComponentHeader * header = new UIComponentHeader(name, enabled);
        s_treeWidget->setItemWidget(item, 0, header);
        item->setExpanded(true);
        //changed = false;
        //return true;
    }
    s_topLevelItemIndex ++;
    s_localItemIndex = 0;
    s_currentHeaderItem = item;
    s_currentHeaderItemIsExpanded = expanded;
    return expanded;
}

bool EditorGUI::Foldout(const std::string &name, UIHeaderState &state)
{
    // hide redundant child items of the last top item
    HideRedundantChildItems();

    QTreeWidgetItem * item;
    bool expanded = true;
    if (s_topLevelItemIndex < s_treeWidget->topLevelItemCount()) // exists, reuse it
    {
        item = s_treeWidget->topLevelItem(s_topLevelItemIndex);
        if (item->isHidden())
        {
            LOG;
            item->setHidden(false);
        }
        expanded = item->isExpanded();
        UIComponentHeader * header = qobject_cast<UIComponentHeader*>(s_treeWidget->itemWidget(item, 0));
        assert(header != nullptr);
        state = header->CheckUpdate(name);
    }
    else
    {
        Debug::LogError("[Foldout] add new QTreeWidgetItem");
        item = new QTreeWidgetItem;
        s_treeWidget->addTopLevelItem(item);
        UIComponentHeader * header = new UIComponentHeader(name);
        s_treeWidget->setItemWidget(item, 0, header);
        item->setExpanded(true);
        //return true;
    }
    s_topLevelItemIndex ++;
    s_localItemIndex = 0;
    s_currentHeaderItem = item;
    s_currentHeaderItemIsExpanded = expanded;
    return expanded;
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


bool FishEditor::EditorGUI::Vector3Field(const std::string &label, Vector3 *v)
{
    //return false;
    UIFloat3 * float3 = CheckNextWidget<UIFloat3>(label, v->x, v->y, v->z);
    return float3->CheckUpdate(label, v->x, v->y, v->z);
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


void EditorGUI::HideRedundantChildItems()
{
    if (s_currentHeaderItem == nullptr || !s_currentHeaderItemIsExpanded)
    {
        return;
    }
    int rowCount = s_currentHeaderItem->childCount();
    for (int i = s_localItemIndex ; i < rowCount; i++)
    {
        auto item = s_currentHeaderItem->child(i);
        if (item->isHidden())
            break;  // do not check the rest of rows
        item->setHidden(true);
        LOG;
        Debug::LogWarning("[HideRedundantChildItems] hide %d", i);
    }
}

