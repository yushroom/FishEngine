#pragma once

#include <Debug.hpp>
using FishEngine::Debug;

#define LOG FishEngine::Debug::LogWarning("%d, %s, %s", __LINE__, __FILE__, __FUNCTION__)