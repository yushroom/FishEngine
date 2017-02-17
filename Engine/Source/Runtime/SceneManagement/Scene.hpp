#pragma once

#include "../FishEngine.hpp"
#include <list>
#include <string>
#include <boost/filesystem.hpp>
#include <memory>

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

            // Returns true if the scene is modified.
            bool isDirty() const;

            // Returns true if the scene is loaded.
            bool isLoaded() const;

            // Returns the name of the scene.
			std::string const & name() const { return m_name;  };

            // Returns the relative path of the scene.
            // Like: "Assets/MyScenes/MyScene.unity".
			std::string path() const { return m_path.string(); }

            // The number of root transforms of this scene.
            int rootCount() const;

            // Returns all the root game objects in the scene.
            std::list<GameObjectPtr> GetRootGameObjects();

            // Whether this is a valid scene.
            // A scene may be invalid if, for example, you tried to open a scene that does not exist.
            // In this case, the scene returned from EditorSceneManager.OpenScene would return False for IsValid.
            bool IsValid() const;

        private:
			std::string m_name;
			boost::filesystem::path m_path;

			enum class LoadingState
			{
				NotLoaded,
				Loading,
				Loaded
			};

        	int m_handle;

            //int m_buildIndex;
        };

		typedef std::shared_ptr<Scene2> Scene2Ptr;
    }
}