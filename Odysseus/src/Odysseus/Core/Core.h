#pragma once
#include <memory>


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
	#if ODC_PLATFORM_WINDOWS
		#define HZ_DEBUGBREAK() __debugbreak()
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
