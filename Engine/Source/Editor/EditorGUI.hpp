#pragma once

#include <type_traits>
#include <stack>

#include "FishEditor.hpp"

#include "UI/UIHeaderState.hpp"
#include <ReflectEnum.hpp>

#include <Color.hpp>
#include <ReflectClass.hpp>

class QTreeWidget;
class QTreeWidgetItem;

namespace FishEditor
{
    class Meta(NonSerializable) EditorGUI
    {
        EditorGUI() = delete;

    public:
        static void Begin();

        static void End();

        // return component name
        static std::string ShowAddComponentMenu();

		static bool BeginComponent(std::string const & componentTypeName, UIHeaderState * outState);
		static bool BeginComponent(std::string const & componentTypeName, bool enabled, UIHeaderState * outState);
		static void EndComponent();

		static bool BeginMaterial(std::string const & materialName);
		static void EndMaterial();

        static bool Button(std::string const & text);

        static bool Toggle(std::string const &label, bool * value);

        static bool ColorField(std::string const & label, FishEngine::Color * color);

        template<typename T>
        static bool EnumPopup(std::string const & label, T * e);

        // index: the index in the array(not the underlying value)
        static bool EnumPopup(std::string const & label, int* index, const char* const* enumStringArray, int arraySize);

        static bool Vector3Field(std::string const & label, FishEngine::Vector3 * v);
		static bool Vector4Field(std::string const & label, FishEngine::Vector4 * v);

        static bool FloatField(std::string const & label, float * v);
        static bool FloatField(std::string const & label, float v);// const version

        static bool Slider(std::string const & label, float * value, float leftValue, float rightValue);

        //static bool ObjectField(const std::string &label, std::shared_ptr<> obj);

        //template< class T >
        static bool ObjectField(const std::string &label, const FishEngine::ObjectPtr &obj);

		static bool TextureField(std::string const & label, FishEngine::TexturePtr * texture);

    private:
        friend class Inspector;
        static QTreeWidget*					s_treeWidget;

		static std::stack<QTreeWidgetItem*> s_itemStack;
		static std::stack<int>				s_itemIndexStack;
		static QTreeWidgetItem *			s_currentItem;
		static QTreeWidgetItem *			s_currentGroupHeaderItem;
		static int							s_currentGroupHeaderItemChildIndex;
		static bool							s_expectNewGroup;

		static void PushGroup();
		static void PopGroup();

        template<class T, class... Args>
        static T* CheckNextWidget(Args&&... args );

		static void HideRedundantChildItemsOfLastGroup();
		static void HideAllChildOfLastItem();
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
