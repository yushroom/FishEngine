#pragma once

#include "Scene.hpp"

namespace FishEngine
{
namespace SceneManagement
{
	class SceneManager
	{
	public:
		static Scene2Ptr CreateScene(std::string const & sceneName);
		
		static Scene2Ptr GetActiveScene();
		
		static Scene2Ptr GetSceneAt(int index);
		//static ScenePtr GetSceneByBuildIndex(int buildIndex);
		static Scene2Ptr GetSceneByName(std::string const & sceneName);
		static Scene2Ptr GetSceneByPath(std::string const & scenePath);

		//static void LoadScene(int sceneBuildIndex, SceneManagement.LoadSceneMode mode = LoadSceneMode.Single);
		//static void LoadScene(string sceneName, SceneManagement.LoadSceneMode mode = LoadSceneMode.Single);
		//static AsyncOperation LoadSceneAsync(string sceneName, SceneManagement.LoadSceneMode mode = LoadSceneMode.Single);
		//static AsyncOperation LoadSceneAsync(int sceneBuildIndex, SceneManagement.LoadSceneMode mode = LoadSceneMode.Single);
		//static void MergeScenes(SceneManagement.Scene sourceScene, SceneManagement.Scene destinationScene);
		//static void MoveGameObjectToScene(GameObject go, Scene scene);
		static bool SetActiveScene(Scene2Ptr scene);

	private:
		static int s_sceneCount;
		static int s_sceneCountInBuildSettings;
		static std::list<Scene2Ptr> s_scenes;
	};
}
}