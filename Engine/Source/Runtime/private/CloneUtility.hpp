#pragma once

#include "../ReflectClass.hpp"
#include <type_traits>
#include <string>
#include <memory>

namespace FishEngine
{
	class Meta(NonSerializable) CloneUtility
	{
	public:

		CloneUtility() = default;

		template<class T>
		inline void Clone(T const & source, T & dest)
		{
			dest = source;
		}
		
//		template<class T>
//		inline void Clone(std::shared_ptr<T> const & source, std::shared_ptr<T> & dest)
//		{
//			static_assert(std::is_base_of<Object, T>::value, "T must be Object");
//			//*dest = *source;
//			source->CopyValueTo(dest, *this);
//		}

		//std::vector<std::weak_ptr<Transform>> m_bones
		template<class T>
		void Clone(std::vector<T> const & source, std::vector<T> & dest)
		{
			dest.resize(source.size());
			for (int i = 0; i < source.size(); ++i)
			{
				this->Clone(source[i], dest[i]);
			}
		}

		void Clone(std::weak_ptr<Transform> const & source, std::weak_ptr<Transform> & dest);
		
		void Clone(FishEngine::TransformPtr const & source, FishEngine::TransformPtr & dest);
		
		void Clone(std::weak_ptr<GameObject> const & source, std::weak_ptr<GameObject> & dest);
		
		void Clone(FishEngine::GameObjectPtr const & source, FishEngine::GameObjectPtr & dest);
		
		void Clone(FishEngine::PrefabPtr const & source, FishEngine::PrefabPtr & dest);
		
//		void Clone(FishEngine::MaterialPtr const & source, FishEngine::MaterialPtr & dest);
//		
//		void Clone(FishEngine::AvatarPtr const & source, FishEngine::AvatarPtr & dest);
//		
//		void Clone(FishEngine::ShaderPtr const & source, FishEngine::ShaderPtr & dest);
//		
//		void Clone(FishEngine::MeshPtr const & source, FishEngine::MeshPtr & dest);
//
//		void Clone(std::string const & source, std::string & dest)
//		{
//			dest = source;
//		}

	//private:

		// instanceID of old object -> cloned object
		std::map<int, ObjectPtr> m_clonedObject;
	};
}
