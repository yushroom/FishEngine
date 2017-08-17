#include <FishEngine/Command.hpp>

namespace FishEngine
{

std::array<Command, MAX_COMMANDS_COUNT> CommandManager::s_commands;
uint8_t CommandManager::s_start = 0;
uint8_t CommandManager::s_end   = 0;
uint8_t CommandManager::s_index = 0;
}
