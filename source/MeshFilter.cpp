#include "MeshFilter.hpp"
#include <imgui/imgui.h>
#include "Mesh.hpp"
#include "EditorGUI.hpp"

NAMESPACE_FISHENGINE_BEGIN

void MeshFilter::OnEditorGUI()
{
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

NAMESPACE_FISHENGINE_END
