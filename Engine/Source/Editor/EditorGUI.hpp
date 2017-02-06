#pragma once

#include <type_traits>


#include "FishEditor.hpp"
#include "UIHeaderState.hpp"

#include <ReflectEnum.hpp>

#include <Color.hpp>

class QTreeWidget;
class QTreeWidgetItem;

namespace FishEditor
{
    class EditorGUI
    {
        EditorGUI() = delete;

    public:
        static void Begin();

        static void End();

        //static void BindGameObject(FishEngine::GameObjectPtr const & go);

        // return component name
        static std::string ShowAddComponentMenu();

#if 1
        // show left checkBox
        // return value: isExpanded
        static bool ComponentGroup(std::string const & name, bool & enabled, UIHeaderState & state);

        // hide left checkBox
        // return value: isExpanded
        static bool ComponentGroup(std::string const & name, UIHeaderState &state);
#endif

        static bool Button(std::string const & text);

        static bool Toggle(std::string const &label, bool * value);

        static bool ColorField(std::string const & label, FishEngine::Color * color);

        template<typename T>
        static bool EnumPopup(std::string const & label, T * e);

        // index: the index in the array(not the underlying value)
        static bool EnumPopup(std::string const & label, int* index, const char* const* enumStringArray, int arraySize);

        static bool Vector3Field(std::string const & label, FishEngine::Vector3 * v);

        static bool FloatField(std::string const & label, float * v);
        static bool FloatField(std::string const & label, float v);// const version

        static bool Slider(std::string const & label, float * value, float leftValue, float rightValue);

        //static bool ObjectField(const std::string &label, std::shared_ptr<> obj);

        //template< class T >
        static bool ObjectField(const std::string &label, const FishEngine::ObjectPtr &obj);

    private:

        static bool MaterialHeader(const std::string &text);

        friend class Inspector;
        static QTreeWidget*     s_treeWidget;
        static int              s_indentLevel;
        static int              s_topLevelItemIndex;
        static int              s_localItemIndex;
        static bool             s_currentHeaderItemIsExpanded;
        static QTreeWidgetItem* s_currentHeaderItem;

        template<class T, class... Args>
        static T* CheckNextWidget(Args&&... args );

        static void StartNewTopItem();
    };

    template<typename T>
    bool EditorGUI::EnumPopup(const std::string &label, T *e)
    {
        static_assert(std::is_enum<T>(), "T must be an enum type");
        int index = FishEngine::EnumToIndex<T>(*e);
        bool changed = EnumPopup(label, &index, FishEngine::EnumToCStringArray<T>(), FishEngine::EnumCount<T>());
        if (changed)
        {
            *e = FishEngine::ToEnum<T>(index);
            return true;
        }
        return false;
    }
}
