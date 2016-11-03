#include "imgui_user.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

extern IMGUI_API ImGuiContext*  GImGui;     // current implicit ImGui context pointer

namespace ImGui
{
    bool ImGui::BeginToolbar(const char* str_id, ImVec2 screen_pos, ImVec2 size)
    {
        bool is_global = GImGui->CurrentWindowStack.Size == 1;
        SetNextWindowPos(screen_pos);
        ImVec2 frame_padding = GetStyle().FramePadding;
        PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
        PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        float padding = frame_padding.y * 2;
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
        if (size.x == 0) size.x = GetContentRegionAvailWidth();
        SetNextWindowSize(size);

        bool ret = is_global ? Begin(str_id, nullptr, size, -1, flags) : BeginChild(str_id, size, false, flags);
        PopStyleVar(3);

        return ret;
    }

    void EndToolbar()
    {
        auto frame_padding = ImGui::GetStyle().FramePadding;
        PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        PushStyleVar(ImGuiStyleVar_WindowPadding, frame_padding);
        PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImVec2 pos = GetWindowPos();
        ImVec2 size = GetWindowSize();
        if (GImGui->CurrentWindowStack.Size == 2) End(); else EndChild();
        PopStyleVar(3);
        ImGuiWindow* win = GetCurrentWindowRead();
        if (GImGui->CurrentWindowStack.Size > 1) SetCursorScreenPos(pos + ImVec2(0, size.y + GetStyle().FramePadding.y * 2));
    }
}
