#pragma once

#include "FishEngine.hpp"
#include "ReflectClass.hpp"
#include "Path.hpp"

class OpenProjectDialog;

namespace FishEngine
{
	// Access to application run-time data.
	// This class contains static methods for looking up information about and controlling the run-time data.
	class Meta(NonSerializable) Application
	{
	public:
		Application() = delete;

		// Contains the path to the game data folder (Read Only).
		// Unity Editor: <path to project folder>/Assets
		// Mac player: <path to player app bundle>/Contents
		// Win/Linux player: <path to executablename_Data folder> (note that most Linux installations will be case-sensitive!)
		static const FishEngine::Path & dataPath() { return s_dataPath; }

		// Are we running inside the Unity editor? (Read Only)
		// Returns true if the game is being run from the Unity editor; false if run from any deployment target.
		static bool isEditor() { return s_isEditor; }

		// Returns true when in any kind of player (Read Only).
		// Returns true in the Unity editor when in play mode.
		static bool isPlaying() { return s_isPlaying; }

	private:
		friend class FishEditor::MainEditor;
		friend class ::MainWindow;
		friend class ::OpenProjectDialog;
		static Path s_dataPath;
		static bool s_isEditor;
		static bool s_isPlaying;
	};
}
