#pragma once

#include <array>
#include <functional>
#include "FishEngine.hpp"
#include "Debug.hpp"

using Command = std::pair<std::function<void(void)>, std::function<void(void)>>;

namespace FishEngine
{

constexpr int MAX_COMMANDS_COUNT = std::numeric_limits<uint8_t>::max();

class CommandManager
{
public:
    CommandManager() = delete;

    static bool Undoable()
    {
        return s_index != s_start;
    }

    static bool Redoable()
    {
        return s_index != s_end;
    }

    static void Undo()
    {
        if (!Undoable())
        {
            Debug::LogWarning("no more command to undo");
            return;
        }
        s_commands[s_index].first();
        s_index--;
    }

    static void Redo()
    {
        if (!Redoable())
        {
            Debug::LogWarning("no more command to redo");
            return;
        }
        s_index++;
        s_commands[s_index].second();
    }

    static void AddCommand(const Command& command)
    {
        s_index++;
        s_commands[s_index] = command;
        s_end = s_index;
        if (s_end == s_start)
            s_start++;
    }

    static void Init()
    {
        s_start = 0;
        s_end = 0;
        s_index = 0;
    }

private:
    static std::array<Command, MAX_COMMANDS_COUNT> s_commands;
    static uint8_t s_start;
    static uint8_t s_end;
    static uint8_t s_index;

};

} // namespace FishEngine
