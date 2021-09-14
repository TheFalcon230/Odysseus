#pragma once
#include "odcpch.h"


#ifdef ODC_PLATFORM_WINDOWS
	#ifdef ODC_BUILD_DLL
		#define ODYSSEUS_API __declspec(dllexport)
	#else
		#define ODYSSEUS_API __declspec(dllimport)
	#endif // ODC_BUILD_DLL
#else
	#error Odysseus only support Winwdow!
#endif // ODC_PLATFORM_WINDOWS

#ifdef ODC_ENABLE_ASSERTS
	#define ODC_ASSERTS(x, ...) {if(!(x)){ ODC_ERROR("Assertion Failed: {0}", __VAR_ARGS__); __debugbreak();}}
	#define ODC_CORE_ASSERT(x, ...) {if(!(x)){ ODC_CORE_ERROR("Assertion Failed: {0}", __VAR_ARGS__); __debugbreak();}}
#else
	#define ODC_ASSERT(x, ...)
	#define ODC_CORE_ASSERT(x, ...)
#endif // ODC_ENABLE_ASSERTS


#define BIT(x) ( 1 << x )
