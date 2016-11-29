#include "imgui_user.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <iostream>

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

    void ImageWithLabel(const char* label, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 /*= ImVec2(0, 0)*/, const ImVec2& uv1 /*= ImVec2(1, 1)*/, const ImVec4& tint_col /*= ImVec4(1, 1, 1, 1)*/, const ImVec4& border_col /*= ImVec4(0, 0, 0, 0)*/)
    {
        auto pos = GetCurrentWindow()->DC.CursorPos;
        Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
        ImRect bb = ImRect(GetItemRectMin(), GetItemRectMax());
        if (border_col.w <= 0)
        {
            bb.Max += ImVec2(2, 2);
        }
        GetCurrentWindow()->DC.CursorPos = pos + ImVec2(0, GetItemRectSize().y);

        auto expeced_size = CalcTextSize(label);

        std::string label_parsed(label);
        if (expeced_size.x > size.x) // overflow
        {
            ImGuiContext& g = *GImGui;
            ImFont* font = g.Font;
            const float font_size = g.FontSize;
            const char* end;
            auto size_3dot = CalcTextSize("...");
            ImVec2 text_size = font->CalcTextSizeA(font_size, size.x - size_3dot.x, 0, label, 0, &end);
            std::string(label, end);
            label_parsed = std::string(label, end) + "...";
        }

        if (border_col.w > 0)
            TextColored(border_col, "%s", label_parsed.c_str());
        else
            Text("%s", label_parsed.c_str());

        bb.Max += ImVec2(0, expeced_size.y);
        GetCurrentWindow()->DC.CursorPos = pos;
        bb.Max += ImVec2(8, 8);
        ItemSize(bb);
        ItemAdd(bb, NULL);
    }
}
