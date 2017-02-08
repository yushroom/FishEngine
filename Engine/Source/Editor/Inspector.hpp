#pragma once

#include "FishEditor.hpp"

using namespace FishEngine;

class InspectorWidget;
class MainWindow;

namespace FishEditor
{

    template< class T>
    struct can_be_enabled : std::integral_constant< bool,
        std::is_base_of<FishEngine::Behaviour, T>::value ||
        std::is_base_of<FishEngine::Renderer, T>::value ||
        std::is_base_of<FishEngine::Collider, T>::value >
    {
    };


    class Inspector
    {
    public:
        Inspector() = delete;

        static void Bind(FishEngine::ObjectPtr const & object);
        static void Bind(FishEngine::GameObjectPtr const & gameObject);
        static void Bind(FishEngine::Material const & material);
        static void Bind(FishEngine::Shader const & shader);
        static void Bind(std::shared_ptr<FishEngine::TextureImporter> const & importer);
        static void Bind(std::shared_ptr<FishEngine::ModelImporter> const & importer);

		static void HideAll();

        template<class T>
        static void OnInspectorGUI(std::shared_ptr<T> const & component);

        static std::string ShowAddComponentMenu();

    private:
        friend class ::MainWindow;

        static void SetInsectorWidget(InspectorWidget* widget);

        static InspectorWidget* s_inspectorWidget;

    public:
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
    };

}
