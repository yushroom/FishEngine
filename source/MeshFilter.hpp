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

        MeshFilter(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {

        }

        std::shared_ptr<Mesh> mesh() const {
            return m_mesh;
        }

        void SetMesh(std::shared_ptr<Mesh> mesh) {
            m_mesh = mesh;
        }

        //virtual void OnInspectorGUI() override;

    private:
        friend class FishEditor::EditorGUI;
        std::shared_ptr<Mesh> m_mesh = nullptr;
    };
}

#endif /* MeshFilter_hpp */
