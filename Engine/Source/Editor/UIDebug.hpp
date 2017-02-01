#pragma once

#include <Debug.hpp>
using FishEngine::Debug;

#define LOG FishEngine::Debug::Log("%d, %s, %s", __LINE__, __FILE__, __FUNCTION__)
