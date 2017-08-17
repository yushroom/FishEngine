#include <FishEngine/Private/CloneUtility.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/Material.hpp>
#include <FishEngine/Avatar.hpp>
#include <FishEngine/Mesh.hpp>
#include <FishEngine/Prefab.hpp>

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
			dest = source;
		}
	}

	void CloneUtility::Clone(TransformPtr const & source, TransformPtr & dest)
	{
		if (source == nullptr)
		{
			dest = nullptr;
			return;
		}

		auto it = m_clonedObject.find(source->GetInstanceID());
		if (it != m_clonedObject.end()) // already cloned
		{
			dest = As<Transform>(it->second);
		}
		else
		{
			dest = source;
		}
	}
	
	void CloneUtility::Clone(std::weak_ptr<GameObject> const & source, std::weak_ptr<GameObject> & dest)
	{
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
			dest = source;
		}
	}
	
	void CloneUtility::Clone(GameObjectPtr const & source, GameObjectPtr & dest)
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
			dest = source;
		}
	}
	
	void CloneUtility::Clone(FishEngine::PrefabPtr const & source, FishEngine::PrefabPtr & dest)
	{
		if (source == nullptr)
		{
			dest = nullptr;
			return;
		}

		auto it = m_clonedObject.find(source->GetInstanceID());
		if (it != m_clonedObject.end()) // already cloned
		{
			dest = As<Prefab>(it->second);
		}
		else
		{
			dest = source;
		}
	}
	
//	void CloneUtility::Clone(FishEngine::MaterialPtr const & source, FishEngine::MaterialPtr & dest)
//	{
//		dest = source;
//	}
//	
//	void CloneUtility::Clone(FishEngine::AvatarPtr const & source, FishEngine::AvatarPtr & dest)
//	{
//		dest = source;
//	}
//	
//	void CloneUtility::Clone(FishEngine::ShaderPtr const & source, FishEngine::ShaderPtr & dest)
//	{
//		dest = source;
//	}
//	
//	void CloneUtility::Clone(FishEngine::MeshPtr const & source, FishEngine::MeshPtr & dest)
//	{
//		dest = source;
//	}
}
