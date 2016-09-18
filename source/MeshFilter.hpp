#ifndef MeshFilter_hpp
#define MeshFilter_hpp

#include "Component.hpp"
#include "GameObject.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "RenderSystem.hpp"
#include "EditorGUI.hpp"

NAMESPACE_FISHENGINE_BEGIN

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
    
    virtual void OnEditorGUI() override {
        if (ImGui::Button("Change")) {
            ImGui::OpenPopup("Select ...");
        }
        EditorGUI::SelectMeshDialogBox([this](Mesh::PMesh mesh)->void {
            this->SetMesh(mesh);
        });
        ImGui::SameLine();
        ImGui::LabelText("Mesh", "%s", m_mesh->name().c_str());
        //ImGui::Text("Mesh: %s", m_mesh->name().c_str());
    }
    
private:
    std::shared_ptr<Mesh> m_mesh = nullptr;
};

NAMESPACE_FISHENGINE_END

#endif /* MeshFilter_hpp */
