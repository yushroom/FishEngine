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
	void CloneUtility::Clone(FishEngine::TransformPtr const & source, FishEngine::TransformPtr & dest)
	{
		//dest = source;
		FishEngine::ObjectPtr obj = dest;
		source->CopyValueTo(obj, *this);
	}
	
	void CloneUtility::Clone(std::weak_ptr<GameObject> const & source, std::weak_ptr<GameObject> & dest)
	{
		//dest = source;
		if (source.expired())
		{
			return;
		}
		auto go = source.lock();
		int oldInstanceId = go->GetInstanceID();
		auto it = m_serializedObject.find(oldInstanceId);
		if (it != m_serializedObject.end())
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
		//dest = source;
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
