#include <Path.hpp>

namespace FishEditor
{
	class EditorApplication
	{
	public:

		// Path to the Unity editor contents folder. (Read Only)
		// The contents folder contains several items internally required by the editor for building players.
		static FishEngine::Path applicationContentsPath();

		// Returns the path to the Unity editor application. (Read Only)
		static FishEngine::Path applicationPath();

		// Is editor currently compiling scripts? (Read Only)
		static bool isCompiling();

		// Is editor currently paused?
		// Let you change pause state programmatically, like pressing the Pause button in the main toolbar.
		static bool isPaused();
		static void setIsPaused();

		// Is editor currently in play mode?
		// Setting isPlaying delays the result until after all script code has completed for this frame.
		static bool isPlaying();
		static void setIsPlaying();

	private:
		friend class MainWindow;
		static FishEngine::Path m_applicationContentsPath;
		static FishEngine::Path m_applicationPath;
	};
}