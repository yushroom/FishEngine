#pragma once

#include "../ReflectClass.hpp"
#include <type_traits>
#include <string>
#include <memory>

namespace FishEngine
{
	Meta(NonSerializable)
	class CloneUtility
	{
	public:

		CloneUtility() = default;

		template<class T, std::enable_if_t<std::is_copy_assignable<T>::value, int> = 0>
		inline void Clone(T const & source, T & dest)
		{
			dest = source;
		}

		template<class T>
		void Clone(std::list<std::shared_ptr<T>> const & source, std::list<std::shared_ptr<T>> & dest)
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be Object");
			for (auto & object : source)
			{
				std::shared_ptr<T> clonedObject = std::dynamic_pointer_cast<T>( object->Clone(*this) );
				dest.push_back(clonedObject);
			}
		}
		
		template<class T>
		inline void Clone(std::shared_ptr<T> const & source, std::shared_ptr<T> & dest)
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be Object");
			//*dest = *source;
			source->CopyValueTo(dest, *this);
		}
		
		void Clone(FishEngine::TransformPtr const & source, FishEngine::TransformPtr & dest);
		
		void Clone(std::weak_ptr<GameObject> const & source, std::weak_ptr<GameObject> & dest);
		
		void Clone(FishEngine::GameObjectPtr const & source, FishEngine::GameObjectPtr & dest);
		
		void Clone(FishEngine::PrefabPtr const & source, FishEngine::PrefabPtr & dest);
		
		void Clone(FishEngine::MaterialPtr const & source, FishEngine::MaterialPtr & dest);
		
		void Clone(FishEngine::AvatarPtr const & source, FishEngine::AvatarPtr & dest);
		
		void Clone(FishEngine::ShaderPtr const & source, FishEngine::ShaderPtr & dest);
		
		void Clone(FishEngine::MeshPtr const & source, FishEngine::MeshPtr & dest);

		void Clone(std::string const & source, std::string & dest)
		{
			dest = source;
		}

	//private:

		// instanceID of old object -> cloned object
		std::map<int, ObjectPtr> m_serializedObject;
	};
}
