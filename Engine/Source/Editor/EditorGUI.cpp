#include "EditorGUI.hpp"

#include <cassert>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <LayerMask.hpp>
#include <ReflectClass.hpp>
#include <ReflectEnum.hpp>
#include <Vector3.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <MeshFilter.hpp>
#include <Mesh.hpp>
#include <MeshRenderer.hpp>
#include <CameraController.hpp>

#include <QTreeWidget>

#include "UI/UIHeader.hpp"
#include "UI/UIFloat3.hpp"
#include "UI/UIFloat.hpp"
#include "UI/UIComboBox.hpp"
#include "UI/UISlider.hpp"
#include "UI/UIObjecField.hpp"
#include "UI/UIColor.hpp"
#include "UI/UIBool.hpp"

#include <generate/Enum_LightType.hpp>
#include <generate/Enum_ShadowCastingMode.hpp>

using namespace FishEditor;
using namespace FishEngine;

#include "UIDebug.hpp"

int                 EditorGUI::s_indentLevel        = 0;
int                 EditorGUI::s_topLevelItemIndex  = 0;
int                 EditorGUI::s_localItemIndex     = 0;
QTreeWidget*        EditorGUI::s_treeWidget         = nullptr;
QTreeWidgetItem*    EditorGUI::s_currentHeaderItem  = nullptr;

template<class T, class... Args>
T* EditorGUI::CheckNextWidget(Args&&... args )
{
    static_assert(std::is_base_of<QWidget, T>(), "T must be a QWidget");

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

template<>
void EditorGUI::OnInspectorGUI(std::shared_ptr<Transform> const & t)
{
    if (Vector3Field("Position", t->m_localPosition))
    {
        t->MakeDirty();
    }

    auto localEulerAngles = t->m_localRotation.eulerAngles();
    if (Vector3Field("Rotation", localEulerAngles))
    {
        t->setLocalEulerAngles(localEulerAngles);
    }

    if (Vector3Field("Scale", t->m_localScale))
    {
        t->MakeDirty();
    }
}

template<>
void EditorGUI::OnInspectorGUI(std::shared_ptr<Camera> const & camera)
{
    // note: must be *static* array
    static const char* listbox_items[] = {
        "Perspective", "Orthographic"
    };
    int list_item_current = camera->m_orthographic ? 1 : 0;
    EnumPopup("Projection", &list_item_current, listbox_items, 2);
    if (camera->m_orthographic != (list_item_current == 1))
    {
        camera->setOrthographic(!camera->m_orthographic);
    }

    if (camera->m_orthographic)
    {
        if (FloatField("Size", &camera->m_orthographicSize))
        {
            camera->m_isDirty = true;
        }
    }
    else
    {
        if (Slider("Field of View", &camera->m_fieldOfView, 1, 179))
        {
            camera->m_isDirty = true;
        }
    }

    if (FloatField("Clipping Planes(Near)", &camera->m_nearClipPlane))
    {
        camera->m_isDirty = true;
    }
    if (FloatField("Clipping Planes(Far)", &camera->m_farClipPlane))
    {
        camera->m_isDirty = true;
    }
}

template<>
void EditorGUI::OnInspectorGUI(const FishEngine::LightPtr& light)
{
    EnumPopup<LightType>("Type", &light->m_type);
    ColorField("Color",         &light->m_color);
    Slider("Intensity",         &light->m_intensity, 0, 8);
    FloatField("Range",         &light->m_range);
    Slider("Bias",              &light->m_shadowBias, 0, 2);
    Slider("Normal Bias",       &light->m_shadowNormalBias, 0, 3);
    Slider("Shadow Near Plane", &light->m_shadowNearPlane, 0.1, 10);
}

template<>
void EditorGUI::OnInspectorGUI(const FishEngine::MeshFilterPtr& meshFilter)
{
    //ObjectPtr obj = meshFilter->m_mesh;
    ObjectField("Mesh", meshFilter->m_mesh);
}

template<>
void EditorGUI::OnInspectorGUI(const FishEngine::RendererPtr& renderer)
{
    EnumPopup("Cast Shadows", &renderer->m_shadowCastingMode);
    Toggle("Receive Shadows", &renderer->m_receiveShadows);
}

template<>
void EditorGUI::OnInspectorGUI(const FishEngine::MeshRendererPtr& renderer)
{
    OnInspectorGUI<Renderer>(renderer);
}

template<>
void EditorGUI::OnInspectorGUI(const std::shared_ptr<CameraController>& renderer)
{
}


void FishEditor::EditorGUI::BindGameObject(const FishEngine::GameObjectPtr &go)
{
    FishEditor::EditorGUI::s_topLevelItemIndex = 0;
    FishEditor::EditorGUI::s_localItemIndex = 0;
    auto t = go->transform();
    if (Foldout("Transform"))
    {
        OnInspectorGUI<Transform>(t);
    }

    int rowCount = s_treeWidget->topLevelItemCount();
    //Debug::LogError("%d", rowCount);
    int componentCount = go->m_components.size() + 1; // +1: transform
    if (rowCount > componentCount) // hide redundant item
    {
        for (int i = componentCount; i < rowCount; i++)
        {
            auto item = s_treeWidget->topLevelItem(i);
            if (item->isHidden())
                break;  // do not check the rest of rows
            item->setHidden(true);
            LOG;
            Debug::LogError("[BindGameObject]hide %d", i);
        }
    }
//    else if (rowCount < componentCount) // add new item if not enough
//    {
//        for (int i = componentCount; i < rowCount; i++)
//        {
//            auto item = new QTreeWidgetItem;
//            treeWidget->addTopLevelItem(item);
//            //UIHeader * header = new UIHeader(name, false, enabled);
//            treeWidget->setItemWidget(item, 0, header);
//            item->setExpanded(true);
//        }
//    }


    for (auto const & comp : go->m_components)
    {
        BeginComponent(comp);
    }
}

bool FishEditor::EditorGUI::Foldout(const std::string &name, bool &enabled, bool &changed)
{
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
        UIHeader * header = qobject_cast<UIHeader*>(s_treeWidget->itemWidget(item, 0));
        assert(header != nullptr);
        changed = header->CheckUpdate(name, enabled);
    }
    else
    {
        Debug::LogError("[Foldout] add new QTreeWidgetItem");
        item = new QTreeWidgetItem;
        s_treeWidget->addTopLevelItem(item);
        UIHeader * header = new UIHeader(name, enabled);
        s_treeWidget->setItemWidget(item, 0, header);
        item->setExpanded(true);
        changed = false;
        //return true;
    }
    s_topLevelItemIndex ++;
    s_localItemIndex = 0;
    s_currentHeaderItem = item;
    return expanded;
}

bool EditorGUI::Foldout(const std::string &name)
{
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
        UIHeader * header = qobject_cast<UIHeader*>(s_treeWidget->itemWidget(item, 0));
        assert(header != nullptr);
        header->CheckUpdate(name);
    }
    else
    {
        Debug::LogError("[Foldout] add new QTreeWidgetItem");
        item = new QTreeWidgetItem;
        s_treeWidget->addTopLevelItem(item);
        UIHeader * header = new UIHeader(name);
        s_treeWidget->setItemWidget(item, 0, header);
        item->setExpanded(true);
        //return true;
    }
    s_topLevelItemIndex ++;
    s_localItemIndex = 0;
    s_currentHeaderItem = item;
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


template<typename T>
bool EditorGUI::EnumPopup(const std::string &label, T *e)
{
    static_assert(std::is_enum<T>(), "T must be an enum type");
    int index = EnumToIndex<T>(*e);
    bool changed = EnumPopup(label, &index, EnumToCStringArray<T>(), EnumCount<T>());
    if (changed)
    {
        *e = ToEnum<T>(index);
        return true;
    }
    return false;
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


bool FishEditor::EditorGUI::Vector3Field(const std::string &label, FishEngine::Vector3 &v)
{
    //return false;
    UIFloat3 * float3 = CheckNextWidget<UIFloat3>(label, v.x, v.y, v.z);
    return float3->CheckUpdate(label, v.x, v.y, v.z);
}


bool EditorGUI::ObjectField(const std::string &label, const ObjectPtr &obj)
{
    UIObjecField * objField = CheckNextWidget<UIObjecField>(label, obj->name());
    return false;
}


void EditorGUI::BeginComponentImpl(const ComponentPtr &component)
{
    Debug::LogError("[BeginComponentImpl] Not Implemented for %s", component->ClassName().c_str());
    if ( Foldout( component->ClassName() ) )
    {
        //OnInspectorGUI<T>(std::static_pointer_cast<T>(component));
        HideRedundantChildItems();
    }
}

// for component derived from behaviour
template<class T, std::enable_if_t<can_be_enabled<T>::value, int>>
void EditorGUI::BeginComponentImpl(FishEngine::ComponentPtr const & component)
{
    //static_assert(std::is_base_of<Behaviour, T>(), "T must be derived from Behaviour");
    auto p = std::static_pointer_cast<T>(component);
    bool enabled = p->enabled();
    bool changed = false;
    bool expanded = Foldout( T::StaticClassName(), enabled, changed );
    if ( expanded )
    {
        OnInspectorGUI<T>(p);
        HideRedundantChildItems();
    }
    if (changed) { p->setEnabled(enabled); }
}

// for component not derived from behaviour
template<class T, std::enable_if_t<!can_be_enabled<T>::value, int>>
void EditorGUI::BeginComponentImpl(FishEngine::ComponentPtr const & component)
{
    static_assert(std::is_base_of<Component, T>(), "T must be derived from Component");
    //static_assert(!std::is_base_of<Behaviour, T>(), "T must *not* be derived from Behaviour");
    if ( Foldout( T::StaticClassName() ) )
    {
        OnInspectorGUI<T>(std::static_pointer_cast<T>(component));
        HideRedundantChildItems();
    }
//    else
//    {
//        Debug::LogError("up");
//    }
}

void EditorGUI::BeginComponent(const ComponentPtr &component)
{
#define CASE(T) case FishEngine::ClassID<T>(): BeginComponentImpl<T>(component); break;
    switch (component->ClassID())
    {
        CASE(Transform)
        CASE(Camera)
        CASE(Light)
        CASE(MeshFilter)
        CASE(MeshRenderer)
        CASE(CameraController)
        default:
            //Foldout( component->ClassName() );
            BeginComponentImpl(component);
    }
#undef CASE

}


void EditorGUI::HideRedundantChildItems()
{
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

