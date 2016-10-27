#ifndef MeshFilter_hpp
#define MeshFilter_hpp

#include "Component.hpp"

namespace FishEngine
{
    class MeshFilter : public Component
    {
    public:
        InjectClassName(MeshFilter)

        MeshFilter() = default;

        MeshFilter(PMesh mesh) : m_mesh(mesh) {

        }

        PMesh mesh() const {
            return m_mesh;
        }

        void SetMesh(PMesh mesh) {
            m_mesh = mesh;
        }

        //virtual void OnInspectorGUI() override;

    private:
        friend class FishEditor::EditorGUI;
        PMesh m_mesh = nullptr;
    };
}

#endif /* MeshFilter_hpp */
