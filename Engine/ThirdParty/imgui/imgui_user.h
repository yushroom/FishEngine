#pragma once

#include "imgui.h"

namespace ImGui
{
    IMGUI_API bool BeginToolbar(const char* str_id, ImVec2 screen_pos, ImVec2 size);
    IMGUI_API void EndToolbar();
}