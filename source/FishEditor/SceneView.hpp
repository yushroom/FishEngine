#ifndef SceneView_hpp
#define SceneView_hpp

#include "FishEditor.hpp"
#include "Camera.hpp"

namespace FishEditor
{
    class SceneView
    {
    public:
        SceneView() = delete;
        
        static void Init();
        static void Update();

        static std::shared_ptr<FishEngine::Camera> camera()
        {
            return m_camera;
        }
        
    private:
        friend class FishEditorWindow;
        friend class EditorRenderSystem;
        static FishEngine::PCamera m_camera;
        static FishEngine::PGameObject m_cameraGameObject;
    };
}

#endif //SceneView_hpp
