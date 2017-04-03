#pragma once

#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>
#include <unordered_map>

#include "../Bounds.hpp"
#include "../ReflectClass.hpp"
#include "../Flags.hpp"

//#define Vector std::vector
#define SPtr std::shared_ptr
#define BS_CORE_EXPORT FE_EXPORT
#define bs_shared_ptr std::shared_ptr
#define bs_shared_ptr_new std::make_shared
#define bs_alloc malloc
#define bs_free free
#define Matrix4 Matrix4x4
#define UnorderedMap std::unordered_map
typedef unsigned char UINT8;
typedef std::uint32_t UINT32;
typedef std::int32_t INT32;
typedef uint64_t UINT64;
typedef std::string String;
typedef FishEngine::Bounds AABox;
