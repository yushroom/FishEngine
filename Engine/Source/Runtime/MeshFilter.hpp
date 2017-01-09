#ifndef MeshFilter_hpp
#define MeshFilter_hpp

#include "Component.hpp"

namespace FishEngine
{
    class FE_EXPORT MeshFilter : public Component
    {
    public:
        InjectClassName(MeshFilter)

        MeshFilter() = default;

        MeshFilter(MeshPtr mesh) : m_mesh(mesh) {

        }

        MeshPtr mesh() const {
            return m_mesh;
        }

        void SetMesh(MeshPtr mesh) {
            m_mesh = mesh;
        }

        //virtual void OnInspectorGUI() override;

    private:
        friend class FishEditor::EditorGUI;
        MeshPtr m_mesh = nullptr;
    };
}

#endif /* MeshFilter_hpp */
