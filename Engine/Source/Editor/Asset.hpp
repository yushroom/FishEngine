#pragma once

#include "FishEditor.hpp"
#include <ReflectClass.hpp>
#include <Path.hpp>

namespace FishEditor
{
	//enum class AssetType
	//{
	//	Model,
	//	Shader,
	//	Material,
	//	AudioClip,
	//};

	// Asset is a collection of imported objects
	// Some asset files may contain multiple objects.
	class Meta(NonSerializable) Asset
	{
	public:

		Asset() = default;
		Asset(Asset const&) = delete;
		Asset& operator=(Asset const &) = delete;

		void Add(FishEngine::ObjectPtr object)
		{
			m_assetObjects.push_back(object);
		}

		FishEngine::ObjectPtr mainObject()
		{
			if (m_assetObjects.size() > 0)
				return m_assetObjects.front();
			return nullptr;
		}

		std::vector<FishEngine::ObjectPtr> m_assetObjects;
		FishEngine::Path m_path;
	};
}