#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64 */
		#define ODC_PLATFORM_WINDOWS 
	#else
		/* Windows x86 */
		#error "x86 builds are not supported!"
	#endif // _WIN64
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define ODC_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MCA == 1
		#define ODC_PLATFORM_MAC
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define	ODC_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define	ODC_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif // End of platform detection



#ifdef ODC_PLATFORM_WINDOWS
		#ifdef ODC_BUILD_DLL
			#define ODYSSEUS_API __declspec(dllexport)
			#define IMGUI_API __declspec(dllexport)
			#define IMPLOT_API __declspec(dllexport)
		#else
			#define ODYSSEUS_API __declspec(dllimport)
			#define IMGUI_API __declspec(dllimport)
			#define IMPLOT_API __declspec(dllimport)
		#endif // ODC_BUILD_DLL
#else
		#error Odysseus only support Winwdow!
#endif // ODC_PLATFORM_WINDOWS

#if ODC_DEBUG
	#if defined(ODC_PLATFORM_WINDOWS)
		#define ODC_DEBUGBREAK() __debugbreak()
	#elif defined(ODC_PLATFORM_LINUX)
		#include <signal.h>
		#define ODC_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif // ODC_PLATFORM_WINDOWS
	#define ODC_ENABLE_ASSERTS
#else
	#define HZ_DEBUGBREAK() 
#endif // ODC_DEBUG





#define BIT(x) ( 1 << x )

#define ODC_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Odysseus
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
