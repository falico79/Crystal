#pragma once

#include "Crystal/Core/Core.h"
#include "Crystal/Core/Log.h"
#include <filesystem>

#ifdef CRYSTALL_ENABLE_ASSERTS

#define CRYSTAL_INTERNAL_ASSERT_IMPL(type,check,msg,...) { if(!(check)) { HZ##type##ERROR(msg, __VA_ARGS__); HZ_DEBUGBREAK(); } }
#define CRYSTAL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CRYSTAL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define CRYSTAL_INTERNAL_ASSERT_NO_MSG(type, check) CRYSTAL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CRYSTAL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define CRYSTAL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define CRYSTAL_INTERNAL_ASSERT_GET_MACRO(...) CRYSTAL_EXPAND_MACRO( CRYSTAL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CRYSTAL_INTERNAL_ASSERT_WITH_MSG, CRYSTAL_INTERNAL_ASSERT_NO_MSG) )

#define CRYSTAL_ASSERT(...) HZ_EXPAND_MACRO( CRYSTAL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define CRYSTAL_CORE_ASSERT(...) HZ_EXPAND_MACRO( CRYSTAL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define CRYSTAL_ASSERT(...)
#define CRYSTAL_CORE_ASSERT(...)
#endif
