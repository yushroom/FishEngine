#include "CloneUtility.hpp"
#include "../GameObject.hpp"
#include "../Transform.hpp"
#include "../Shader.hpp"
#include "../Material.hpp"
#include "../Avatar.hpp"
#include "../Mesh.hpp"
#include "../Prefab.hpp"

namespace FishEngine
{
	void CloneUtility::Clone(std::weak_ptr<Transform> const & source, std::weak_ptr<Transform> & dest)
	{
		if (source.expired())
		{
			dest.reset();
			return;
		}

		auto sourceTransform = source.lock();
		auto it = m_clonedObject.find(sourceTransform->GetInstanceID());
		if (it != m_clonedObject.end()) // already cloned
		{
			dest = As<Transform>(it->second);
		}
		else
		{
			abort();
			//auto sourceGameObject = 
			auto parent = sourceTransform->parent();
			if (parent == nullptr)
			{
				abort();
			}

			//TODO
			dest = source;
		}
	}

	void CloneUtility::Clone(FishEngine::TransformPtr const & source, FishEngine::TransformPtr & dest)
	{
		if (source == nullptr)
		{
			dest = nullptr;
			return;
		}
		source->CopyValueTo(dest, *this);

		for (auto & childTransform : source->children())
		{
			auto childGameObject = childTransform->gameObject();
			int sourceInstanceID = childTransform->GetInstanceID();
			auto it = m_clonedObject.find(sourceInstanceID);
			if (it == m_clonedObject.end())	// not serialized
			{
				auto clonedGameObject = std::dynamic_pointer_cast<GameObject>(childTransform->gameObject()->Clone(*this));
				auto clonedTransform = clonedGameObject->transform();
				m_clonedObject[childGameObject->GetInstanceID()] = clonedGameObject;
				m_clonedObject[childTransform->GetInstanceID()] = clonedTransform;
				dest->children().push_back(clonedGameObject->transform());
			}
			else
			{
				abort();
			}
		}
	}
	
	void CloneUtility::Clone(std::weak_ptr<GameObject> const & source, std::weak_ptr<GameObject> & dest)
	{
		//dest = source;
		if (source.expired())
		{
			dest.reset();
			return;
		}
		auto go = source.lock();
		int sourceInstanceID = go->GetInstanceID();
		auto it = m_clonedObject.find(sourceInstanceID);
		if (it != m_clonedObject.end())
		{
			dest = std::dynamic_pointer_cast<GameObject>( it->second );
		}
		else
		{
			abort();
		}
	}
	
	void CloneUtility::Clone(FishEngine::GameObjectPtr const & source, FishEngine::GameObjectPtr & dest)
	{
		if (source == nullptr)
		{
			dest = nullptr;
			return;
		}
		int sourceInstanceID = source->GetInstanceID();
		auto it = m_clonedObject.find(sourceInstanceID);
		if (it != m_clonedObject.end())
		{
			dest = std::dynamic_pointer_cast<GameObject>(it->second);
		}
		else
		{
			abort();
		}
	}
	
	void CloneUtility::Clone(FishEngine::PrefabPtr const & source, FishEngine::PrefabPtr & dest)
	{
		dest = source;
	}
	
	void CloneUtility::Clone(FishEngine::MaterialPtr const & source, FishEngine::MaterialPtr & dest)
	{
		dest = source;
	}
	
	void CloneUtility::Clone(FishEngine::AvatarPtr const & source, FishEngine::AvatarPtr & dest)
	{
		dest = source;
	}
	
	void CloneUtility::Clone(FishEngine::ShaderPtr const & source, FishEngine::ShaderPtr & dest)
	{
		dest = source;
	}
	
	void CloneUtility::Clone(FishEngine::MeshPtr const & source, FishEngine::MeshPtr & dest)
	{
		dest = source;
	}
}
