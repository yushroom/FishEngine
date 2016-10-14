#ifndef Scene_hpp
#define Scene_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class Scene
    {
    public:

        Scene() = delete;

        static PGameObject CreateGameObject(const std::string& name);

        static void Init();
        static void Start();
        static void Update();
        static void RenderShadow(PLight& light);
        static void OnDrawGizmos();
        //static void Render();

        static PGameObject Find(const std::string& name);

        static void DestroyImmediate(PGameObject g);
        static void DestroyImmediate(PComponent c);
        static void DestroyImmediate(PScript s);

        static void Destroy(PGameObject obj,    const float t = 0.0f);
        static void Destroy(PComponent c,       const float t = 0.0f);
        static void Destroy(PScript s,          const float t = 0.0f);

    private:
        friend class RenderSystem;
        friend class FishEditor::EditorGUI;
        friend class FishEditor::EditorRenderSystem;
        static std::list<PGameObject>   m_gameObjects;
        static std::vector<PGameObject> m_gameObjectsToBeDestroyed;
        static std::vector<PComponent>  m_componentsToBeDestroyed;
        static std::vector<PScript>     m_scriptsToBeDestroyed;
    };
}

#endif /* Scene_hpp */
