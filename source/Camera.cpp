#include "Camera.hpp"
#include "Scene.hpp"

NAMESPACE_FISHENGINE_BEGIN

std::shared_ptr<Camera> Camera::main()
{
    return Scene::mainCamera();
}

NAMESPACE_FISHENGINE_END
