#pragma once

#include "Odysseus/Core/Core.h"
#include "Odysseus/Core/Log.h"
#include <filesystem>

#ifdef ODC_ENABLE_ASSERTS

#define ODC_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ODC##type##ERROR(msg, __VA_ARGS__); ODC_DEBUGBREAK(); } }
#define ODC_INTERNAL_ASSERT_WITH_MSG(type, check, ...) ODC_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define ODC_INTERNAL_ASSERT_NO_MSG(type, check) ODC_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", ODC_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define ODC_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define ODC_INTERNAL_ASSERT_GET_MACRO(...) ODC_EXPAND_MACRO(ODC_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ODC_INTERNAL_ASSERT_WITH_MSG, ODC_INTERNAL_ASSERT_NO_MSG))

#define ODC_ASSERT(...) ODC_EXPAND_MACRO(ODC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define ODC_CORE_ASSERT(...) ODC_EXPAND_MACRO(ODC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define ODC_ASSERT(x, ...)
#define ODC_CORE_ASSERT(x, ...)
#endif // ODC_ENABLE_ASSERTS
