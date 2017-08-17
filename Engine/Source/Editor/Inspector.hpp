#pragma once

#include "FishEditor.hpp"
#include <FishEngine/ReflectClass.hpp>

using namespace FishEngine;

class InspectorWidget;
class MainWindow;
class QAction;

namespace FishEditor
{

	template< class T>
	struct can_be_enabled : std::integral_constant< bool,
		std::is_base_of<FishEngine::Behaviour, T>::value ||
		std::is_base_of<FishEngine::Renderer, T>::value ||
		std::is_base_of<FishEngine::Collider, T>::value >
	{
	};


	class Meta(NonSerializable) Inspector
	{
	public:
		Inspector() = delete;

		static void Bind(FishEngine::ObjectPtr const & object);
		static void Bind(FishEngine::GameObjectPtr const & gameObject);
		static void Bind(FishEngine::Material const & material);
		static void Bind(FishEngine::Shader const & shader);
		static void Bind(TextureImporterPtr const & importer);
		static void Bind(ModelImporterPtr const & importer);

		static void HideAll();

		template<class T>
		static void OnInspectorGUI(std::shared_ptr<T> const & component);

		static std::string ShowAddComponentMenu();
		static QAction* ShowComponentMenu();

	private:
		friend class ::MainWindow;

		static void SetInsectorWidget(InspectorWidget* widget);

		static InspectorWidget* s_inspectorWidget;

		static std::weak_ptr<FishEngine::Component> s_targetComponent;

		

	public:
		static void BeginComponent(FishEngine::ComponentPtr const & component);

		static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

		template<class T, std::enable_if_t<can_be_enabled<T>::value, int> = 0>
		static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

		template<class T, std::enable_if_t<!can_be_enabled<T>::value, int> = 0>
		static void BeginComponentImpl(FishEngine::ComponentPtr const & component);

		template<class T, std::enable_if_t<!std::is_base_of<FishEngine::Component, T>::value, int> = 0>
		static void BeginComponentImpl(FishEngine::ComponentPtr const & component) = delete;
	};

}
