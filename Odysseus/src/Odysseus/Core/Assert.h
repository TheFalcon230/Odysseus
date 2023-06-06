#pragma once

#include "Odysseus/Core/Core.h"
#include "Odysseus/Core/Log.h"
#include <filesystem>

#ifdef ODC_ENABLE_ASSERTS
#define ODC_ASSERTS(x, ...) {if(!(x)){ ODC_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK();}}
#define ODC_CORE_ASSERT(x, ...) {if(!(x)){ ODC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK();}}
#else
#define ODC_ASSERT(x, ...)
#define ODC_CORE_ASSERT(x, ...)
#endif // ODC_ENABLE_ASSERTS
