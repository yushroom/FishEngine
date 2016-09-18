#ifndef SceneView_hpp
#define SceneView_hpp

#include "FishEdtor.hpp"
#include "Camera.hpp"

NAMESPACE_FISHEDITOR_BEGIN

class SceneView
{
public:
    SceneView() = delete;

    static FishEngine::Camera& camera() {
        return m_camera;
    }
    

private:
    static FishEngine::Camera m_camera;
};

NAMESPACE_FISHEDITOR_END

#endif //SceneView_hpp
