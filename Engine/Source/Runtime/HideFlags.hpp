#pragma once

namespace FishEngine
{
	enum class HideFlags
	{
		//A normal, visible object. This is the default.
		None,
		
		//The object will not appear in the hierarchy.
		HideInHierarchy,
		
		//It is not possible to view it in the inspector.
		HideInInspector,
		
		//The object will not be saved to the scene in the editor.
		DontSaveInEditor,
		
		//The object is not be editable in the inspector.
		NotEditable,
		
		//The object will not be saved when building a player.
		DontSaveInBuild,
		
		//The object will not be unloaded by Resources.UnloadUnusedAssets.
		DontUnloadUnusedAsset,
		
		//The object will not be saved to the scene. It will not be destroyed when a new scene is loaded. It is a shortcut for HideFlags.DontSaveInBuild | HideFlags.DontSaveInEditor | HideFlags.DontUnloadUnusedAsset.
		DontSave,
		
		//A combination of not shown in the hierarchy, not saved to to scenes and not unloaded by The object will not be unloaded by Resources.UnloadUnusedAssets.
		HideAndDontSave,
	};
}

//#include "HideFlags.generate.hpp"