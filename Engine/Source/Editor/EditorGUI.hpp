#pragma once

#include <type_traits>


#include "FishEditor.hpp"
#include <Color.hpp>

class QTreeWidget;
class QTreeWidgetItem;

namespace FishEditor
{
    template< class T>
    struct can_be_enabled : std::integral_constant< bool,
            std::is_base_of<FishEngine::Behaviour, T>::value ||
            std::is_base_of<FishEngine::Renderer, T>::value >
    {
    };

    class EditorGUI
    {
        EditorGUI() = delete;

    public:

        static QTreeWidget*     s_treeWidget;

        static int              s_indentLevel;

        static void BindGameObject(FishEngine::GameObjectPtr const & go);

    private:

        static int              s_topLevelItemIndex;
        static int              s_localItemIndex;
        static QTreeWidgetItem* s_currentHeaderItem;

        template<class T, class... Args>
        static T* CheckNextWidget(Args&&... args );

        // show left checkBox
        // return value: isExpanded
        static bool Foldout(std::string const & name, bool & enabled, bool & changed);

        // hide left checkBox
        // return value: isExpanded
        static bool Foldout(std::string const & name);

        static bool Toggle(const std::string &label, bool * value);

        static bool ColorField(std::string const & label, FishEngine::Color * color);

        template<typename T>
        static bool EnumPopup(std::string const & label, T * e);

        // index: the index in the array(not the underlying value)
        static bool EnumPopup(std::string const & label, int* index, const char* const* enumStringArray, int arraySize);

        static bool Vector3Field(std::string const & label, FishEngine::Vector3 & v);

        static bool FloatField(std::string const & label, float * v);
        static bool FloatField(std::string const & label, float v);// const version

        static bool Slider(std::string const & label, float * value, float leftValue, float rightValue);

        //static bool ObjectField(const std::string &label, std::shared_ptr<> obj);

        //template< class T >
        static bool ObjectField(const std::string &label, const FishEngine::ObjectPtr &obj);

//        static void OnInspectorGUI(FishEngine::ComponentPtr const & component)
//        {
//        }

        template<class T>
        static void OnInspectorGUI(std::shared_ptr<T> const & component);

        static void BeginComponent(FishEngine::ComponentPtr const & component);

        static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

        // for component derived from behaviour
        template<class T, std::enable_if_t<can_be_enabled<T>::value, int> = 0>
        static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

        // for component not derived from behaviour
        template<class T, std::enable_if_t<!can_be_enabled<T>::value, int> = 0>
        static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

        template<class T, std::enable_if_t<!std::is_base_of<FishEngine::Component, T>::value, int> = 0>
        static void BeginComponentImpl(FishEngine::ComponentPtr const & component) = delete;

        static void HideRedundantChildItems();
    };
}
