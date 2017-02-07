#pragma once

#include "../FishEngine.hpp"

namespace FishEngine
{
    namespace SceneManagement
    {
        // Run-time data structure for *.unity file.
        struct Scene2
        {
        public:
            // Returns the index of the scene in the Build Settings.
            // Always returns -1 if the scene was loaded through an AssetBundle.
            int buildIndex() const;

            // Returns true if the scene is modifed.
            bool isDirty() const;

            // Returns true if the scene is loaded.
            bool isLoaded() const;

            // Returns the name of the scene.
            std::string const & name() const;

            // Returns the relative path of the scene.
            // Like: "Assets/MyScenes/MyScene.unity".
            std::stirng const & path() const;

            // The number of root transforms of this scene.
            int rootCount() const;

            // Returns all the root game objects in the scene.
            std::vector<GameObjectPtr> GetRootGameObjects();

            // Whether this is a valid scene.
            // A scene may be invalid if, for example, you tried to open a scene that does not exist.
            // In this case, the scene returned from EditorSceneManager.OpenScene would return False for IsValid.
            bool IsValid() const;

        private:
        	enum class LoadingState
        	{
        		NotLoaded,
        		Loading,
        		Loaded
        	}

        	int m_handle;

            //int m_buildIndex;
        };
    }
}