#pragma once

#include <functional>
#include "FishEditor.hpp"

namespace FishEditor
{
	/// <summary>
	///   <para>Flags for the PrefabUtility.ReplacePrefab function.</para>
	/// </summary>
	enum class ReplacePrefabOptions
	{
		/// <summary>
		///   <para>Replaces prefabs by matching pre-existing connections to the prefab.</para>
		/// </summary>
		Default,
		/// <summary>
		///   <para>Connects the passed objects to the prefab after uploading the prefab.</para>
		/// </summary>
		ConnectToPrefab,
		/// <summary>
		///   <para>Replaces the prefab using name based lookup in the transform hierarchy.</para>
		/// </summary>
		ReplaceNameBased
	};

	// Utility class for any prefab related operations.
	class PrefabUtility
	{
	public:

		// Delegate for method that is called after prefab instances in the scene have been updated.
		static std::function<void(FishEngine::GameObjectPtr)> prefabInstanceUpdated;

		//static FishEngine::GameObjectPtr CreatePrefab(std::string const & path, GameObjectPtr go, Re)
	};
}